#include <dbflib.hpp>
#include <iostream>
#include <assert.h>

int main(int argc, char const *argv[]) {
    dbflib::DBFileBuilder builder{ dbflib::DBFBO_ALIGN };

    struct TestLink3 {
        int val3;
    };
    struct TestLink2 {
        TestLink3* link3;
        int val2;
    };
    struct TestLink1 {
        int val1;
    };
    struct TestLinkRoot {
        TestLink1* link1;
        TestLink2* link2;
        int valr;
    };

    auto [rootId, p1] = builder.CreateBlock<TestLinkRoot>();
    p1->valr = 65;

    auto [link1Id, p2] = builder.CreateBlock<TestLink1>();
    p2->val1 = 42;
    builder.CreateLink(rootId, offsetof(TestLinkRoot, link1), link1Id);


    auto [link2Id, p3] = builder.CreateBlock<TestLink2>();
    p3->val2 = 85;
    builder.CreateLink(rootId, offsetof(TestLinkRoot, link2), link2Id);

    auto [link3Id, p4] = builder.CreateBlock<TestLink3>();

    builder.GetBlock<TestLink3>(link3Id)->val3 = 34;

    builder.CreateLink(link2Id, offsetof(TestLink2, link3), link3Id);

    dbflib::DB_FILE* file = builder.Build();


    auto ValidateFile = [](dbflib::DB_FILE* file, const char* type) {
        TestLinkRoot* root = file->Start<TestLinkRoot>();
        assert(root->valr == 65 && "Bad link root value");
        assert(root->link1->val1 == 42 && "Bad link 1 value");
        assert(root->link2->val2 == 85 && "Bad link 2 value");
        assert(root->link2->link3->val3 == 34 && "Bad link 2/3 value");

        std::cout << "ok for " << type << "\n";
    };

    auto Validate = [&ValidateFile](dbflib::DBFileReader& reader, const char* type) {
        ValidateFile(reader.GetFile(), type);
    };
    std::filesystem::path tmp = "test.bin";

    builder.WriteToFile(tmp);

    // test fastlink
    file->Validate();
    assert(file->Link() && "the file wasn't linked!");
    assert(!file->Link() && "the file was linked more than once!");
    assert(!file->Link() && "the file was linked more than once!");
    assert(!file->Link() && "the file was linked more than once!");
    assert(file->Link(true) && "the file wasn't linked!");
    ValidateFile(file, "link");

    dbflib::DBFileReader reader{ file };
    Validate(reader, "buffer");

    dbflib::DBFileReader readerFile{ tmp };
    Validate(readerFile, "file");

    std::filesystem::remove(tmp);

    return 0;
}
