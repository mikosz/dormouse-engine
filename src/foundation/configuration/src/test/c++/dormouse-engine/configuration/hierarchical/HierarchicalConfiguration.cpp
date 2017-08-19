#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <algorithm>
#include <iterator>
#include <set>
#include <functional>

#include "dormouse-engine/configuration/hierarchical/HierarchicalConfiguration.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::configuration;
using namespace dormouse_engine::configuration::hierarchical;

BOOST_AUTO_TEST_SUITE(HierachicalConfigurationTestSuite);

BOOST_AUTO_TEST_CASE(BuildsAConfigurationHierarchy) {
	HierarchicalConfigurationSharedPtr configuration = HierarchicalConfiguration::create();

	HierarchicalConfiguration::Node father1(HierarchicalConfiguration::create());
	father1->add("child", HierarchicalConfiguration::create("son1-1"));
	father1->add("child", HierarchicalConfiguration::create("son1-2"));
	father1->add("child", HierarchicalConfiguration::create("son1-3"));

	HierarchicalConfiguration::Node father2(HierarchicalConfiguration::create());
	father2->add("child", HierarchicalConfiguration::create("son2-1"));
	father2->add("child", HierarchicalConfiguration::create("son2-2"));

	HierarchicalConfiguration::Node grandfather(HierarchicalConfiguration::create());
	grandfather->add("parent", father1);
	grandfather->add("parent", father2);

	configuration->set("grandfather", grandfather);

	HierarchicalConfiguration::Nodes parents;
	configuration->getAll("grandfather/parent", &parents);
	BOOST_CHECK_EQUAL(parents.size(), 2);

	HierarchicalConfiguration::Nodes children;
	configuration->getAll("grandfather/parent/child", &children);

	std::set<std::string> childrenGot;
	std::transform(
			children.begin(),
			children.end(),
			std::inserter(childrenGot, childrenGot.end()),
			std::bind(&HierarchicalConfiguration::text, std::placeholders::_1)
			);
	std::set<std::string> childrenExpected;
	childrenExpected.insert("son1-1");
	childrenExpected.insert("son1-2");
	childrenExpected.insert("son1-3");
	childrenExpected.insert("son2-1");
	childrenExpected.insert("son2-2");
	BOOST_CHECK_EQUAL_COLLECTIONS(
			childrenGot.begin(),
			childrenGot.end(),
			childrenExpected.begin(),
			childrenExpected.end()
			);
}

BOOST_AUTO_TEST_CASE(CantAddANodeWithNonUniqueParent) {
	HierarchicalConfigurationSharedPtr configuration = HierarchicalConfiguration::create();
	configuration->set("grandfather", HierarchicalConfiguration::create());
	configuration->set("grandfather/parent", HierarchicalConfiguration::create());
	configuration->add("grandfather/parent", HierarchicalConfiguration::create());
	BOOST_CHECK_THROW(
			configuration->add("grandfather/parent/child", HierarchicalConfiguration::create()),
			MultipleValuesWhereSingleValueRequired
			);
}

BOOST_AUTO_TEST_CASE(CantAddANodeWithEmptyPath) {
	HierarchicalConfigurationSharedPtr configuration = HierarchicalConfiguration::create();
	BOOST_CHECK_THROW(
			configuration->add("", HierarchicalConfiguration::create()),
			node::NonEmptyPathExpected
			);
}

BOOST_AUTO_TEST_CASE(CantSetNodeWithNonUniqueParent) {
	HierarchicalConfigurationSharedPtr configuration = HierarchicalConfiguration::create();
	configuration->set("grandfather", HierarchicalConfiguration::create());

	HierarchicalConfiguration::Node father1(HierarchicalConfiguration::create());
	father1->add("child", HierarchicalConfiguration::create());

	HierarchicalConfiguration::Node father2(HierarchicalConfiguration::create());
	father2->add("child", HierarchicalConfiguration::create());

	configuration->set("grandfather/parent", father1);
	configuration->add("grandfather/parent", father2);
	BOOST_CHECK_THROW(
			configuration->set("grandfather/parent/son", HierarchicalConfiguration::create()),
			MultipleValuesWhereSingleValueRequired
			);

	HierarchicalConfiguration::Nodes sons;
	configuration->getAll("grandfather/parent/child", &sons);
	BOOST_CHECK_EQUAL(sons.size(), 2);
}

BOOST_AUTO_TEST_CASE(CantSetNodeWithEmptyPath) {
	HierarchicalConfigurationSharedPtr configuration = HierarchicalConfiguration::create();
	BOOST_CHECK_THROW(
			configuration->set("", HierarchicalConfiguration::create()),
			node::NonEmptyPathExpected
			);
}

BOOST_AUTO_TEST_CASE(CantEraseNodeWithEmptyPath) {
	HierarchicalConfigurationSharedPtr configuration = HierarchicalConfiguration::create();
	BOOST_CHECK_THROW(configuration->erase(""), node::NonEmptyPathExpected);
}

BOOST_AUTO_TEST_CASE(NodePathIsSelectsApplicableNode) {
	auto configuration = HierarchicalConfiguration::create();
	configuration->set("grandfather", HierarchicalConfiguration::create());

	auto father1 = HierarchicalConfiguration::create();
	auto child1 = HierarchicalConfiguration::create();
	child1->add("id", HierarchicalConfiguration::create("id-1"));
	father1->add("child", child1);

	configuration->add("grandfather/father", father1);

	auto father2 = HierarchicalConfiguration::create();
	auto child2 = HierarchicalConfiguration::create();
	child2->add("id", HierarchicalConfiguration::create("id-2"));
	father2->add("child", child2);

	configuration->add("grandfather/father", father2);

	auto found = configuration->get(
		(node::Path() / "grandfather/father")[node::Path("child/id").is("id-2")]
		);

	BOOST_CHECK_EQUAL(*father2, *found);
}

BOOST_AUTO_TEST_CASE(ParentPathCanContainSelectorsWhenAdding) {
	auto configuration = HierarchicalConfiguration::create();
	configuration->set("grandfather", HierarchicalConfiguration::create());

	auto father1 = HierarchicalConfiguration::create();
	auto child1 = HierarchicalConfiguration::create();
	child1->add("id", HierarchicalConfiguration::create("id-1"));
	father1->add("child", child1);

	configuration->add("grandfather/father", father1);

	auto father2 = HierarchicalConfiguration::create();
	auto child2 = HierarchicalConfiguration::create();
	child2->add("id", HierarchicalConfiguration::create("id-2"));
	father2->add("child", child2);

	configuration->add("grandfather/father", father2);

	auto path = (node::Path() / "grandfather/father")[node::Path("child/id").is("id-2")] / "child";

	configuration->add(path / "name", HierarchicalConfiguration::create("Johnny"));

	BOOST_CHECK_EQUAL(configuration->get(path / "name")->text(), "Johnny");
}

BOOST_AUTO_TEST_CASE(AddedNodePathCantContainSelectorsWhenAdding) {
	auto configuration = HierarchicalConfiguration::create();
	BOOST_CHECK_THROW(
		configuration->add((node::Path() / "node").is("text"), HierarchicalConfiguration::create()),
		AddOrSetNodePathChildHasSelector
		);
}

BOOST_AUTO_TEST_CASE(AddedNodePathCantContainSelectorsWhenSetting) {
	auto configuration = HierarchicalConfiguration::create();
	BOOST_CHECK_THROW(
		configuration->set((node::Path() / "node").is("text"), HierarchicalConfiguration::create()),
		AddOrSetNodePathChildHasSelector
		);
}

BOOST_AUTO_TEST_SUITE_END(/* HierachicalConfigurationTestSuite */);
