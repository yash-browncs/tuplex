
#include "gtest/gtest.h"
#include <Context.h>
#include "TestUtils.h"
#include <random>

#include "gtest/gtest.h"
#include <AnnotatedAST.h>
#include <graphviz/GraphVizGraph.h>
#include "../../codegen/include/parser/Parser.h"

class IsKeywordTest : public PyTest {};

bool saveToPDFx(tuplex::ASTNode* root, const std::string& path) {
    using namespace tuplex;
    assert(root);
    GraphVizGraph graph;
    graph.createFromAST(root);
    return graph.saveAsPDF(path);
}

// works
TEST_F(IsKeywordTest, xIsNone) {
    using namespace tuplex;
    auto code = "lambda x: x is None";
    auto node = std::unique_ptr<ASTNode>(tuplex::parseToAST(code));
    ASSERT_TRUE(node);
    printParseTree(code, std::cout);
    saveToPDFx(node.get(), "xIsNone.pdf");
}


TEST_F(IsKeywordTest, OptionBoolIsBool) {
    using namespace tuplex;

    Context c(microTestOptions());
    Row row1(Field::null());
    Row row2(Field::null());
    Row row3(Field::null());
    Row row5(Field::null());

    auto code = "lambda x: x is False";
    auto m = c.parallelize({row1, row2, row3, row5})
            .map(UDF(code)).collectAsVector();

    EXPECT_EQ(m.size(), 2);
    for(int i = 0; i < m.size(); i++) {
        assert(m[i].toPythonString() == "(False,)");
    }
    

}

TEST_F(IsKeywordTest, BoolIsBool) {
    using namespace tuplex;

    Context c(microTestOptions());
    Row row1(Field(true));
    Row row2(Field(false));

    auto code = "lambda x: x is False";
    auto m = c.parallelize({row1, row2})
            .map(UDF(code)).collectAsVector();

    EXPECT_EQ(m.size(), 2);
    for(int i = 0; i < m.size(); i++) {
        assert(m[i].toPythonString() == "(False,)");
    }
}
