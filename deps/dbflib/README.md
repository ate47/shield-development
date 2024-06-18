# Dynamic binary file

Library to load and create 64 bits dynamically linked binary files.

**Table of contents**

- [Dynamic binary file](#dynamic-binary-file)
  - [Import library](#import-library)
  - [File structure](#file-structure)
    - [Header](#header)
    - [Links](#links)
  - [Usage](#usage)
    - [Read file](#read-file)
    - [Create file](#create-file)


## Import library

Only the header file in `src/lib` is required for this library.

## File structure

All the numbers are described in little endian.

### Header

The file starts with the header, it is describing the different components of the file.

```cpp
struct DB_FILE {
    uint64_t magic = 0x0d0a46424424;
    uint8_t version;
    uint8_t flags;
    uint16_t links_count;
    uint32_t links_table_offset;
    uint32_t start_offset;
    uint32_t data_size;
    uint32_t file_size;
    uint32_t __pad;
    void* last_link;
};
```

| Name                 | Min version | Description                       |
| -------------------- | ----------- | --------------------------------- |
| `magic`              | 0           | File magic                        |
| `version`            | 0           | The compiled version of this file |
| `flags`              | 0           | Compiler dependent flags          |
| `links_count`        | 0           | Count of links in the links table |
| `links_table_offset` | 0           | Offset of the links table         |
| `start_offset`       | 0           | Offset of the start               |
| `data_size`          | 0           | data section size                 |
| `file_size`          | 0           | File size                         |
| `last_link`          | 0           | Internal runtime object           |

### Links

The links location is described in the header by the field `links_table_offset`, the count is described in the header by the field `links_count` and each links is defined by this structure:

```cpp
struct DB_FILE_LINK {
    uint32_t origin;
    uint32_t destination;
};
```

`origin` is describing the origin offset inside the file and `destination` the destination offset. It can be read as

```cpp
*(void**)&file[origin] = &file[destination];
```

## Usage

First include the library in your application

```cpp
#include <dbflib.hpp>
```

### Read file

The file reading can be done by reading a buffer containing the data or by reading a file using the type `dbflib::DBFileReader`.

```cpp
// create by reading a file
dbflib::DBFileReader reader{ "path/to/your/file" };

// create by reading a buffer, a size can be given to avoid overflows
dbflib::DBFileReader reader{ buffer, bufferSize };
```

The file will be linked and a pointer to the data can be obtained using

```cpp
MyType* data = (MyType*)reader.GetStart();
```

The pointer requires is bound to the reader for file reading, otherwise it is bound to the buffer for buffer reading.

If the might be moved, the method `Link` can be used on the file to link (if required) the file again.

```cpp
dbflib::DB_FILE* buffer;
// ...

buffer->Link();
MyType* data = buffer->Start<MyType>();
```

### Create file

The file creation can be done using the type `dbflib::DBFileReader`.

```cpp
dbflib::DBFileBuilder builder{};
```

You can create data blocks using the `CreateBlock` method, it can be used using a buffer or to directly create a memory block. It returns a block id.

A link can be create between 2 blocks to link them, it is done using the `CreateLink` method.


For example, let's take this small structure
```cpp
struct DemoLinkSub {
    int valsub;
};
struct DemoLinkRoot {
    int valroot;
    DemoLinkSub* link;
};
```

We want to link `DemoLinkSub` to `DemoLinkRoot` using the link field.

For that we first create the 2 blocks.

```cpp
auto [rootId, root] = builder.CreateBlock<DemoLinkRoot>();
// we can init the block data
root->valroot = 42;

auto [subId, sub] = builder.CreateBlock<DemoLinkSub>();
sub->valsub = 32;
```

Note that the `CreateBlock` method is returning a pointer to the created block, this pointer is valid until we create another block. To get the pointer after another block creation, the `GetBlock` method can be used.

```cpp
DemoLinkRoot* rootBlock = builder.GetBlock<DemoLinkRoot>(rootId);
```

This pointer will also be valid until another block is created.

To add a link, we use the `CreateLink` method.

```cpp
builder.CreateLink(rootId, offsetof(DemoLinkRoot, link), subId);
```

Once all the blocks are created, we can build the file. It can be done using the `Build` method or directly by using the `WriteToFile`.


```cpp
// build and return a pointer
dbflib::DB_FILE* buffer = Build();

// build and write the object into a file
builder.WriteToFile("path/to/your/file");
```
