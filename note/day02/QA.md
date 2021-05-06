## GetMemoryMapの仕様について
UEFIの仕様書を見ると、gBS->GetMemoryMap()の仕様では
```
typedef
EFI_STATUS
(EFIAPI *EFI_GET_MEMORY_MAP) (
    IN OUT UINTN *MemoryMapSize,
    OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
    OUT UINTN *MapKey,
    OUT UINTN *DescriptorSize,
    OUT UINT32 *DescriptorVersion
 );
 ```
 と、`*MemoryMap`は`OUT`のみが指定されていますが、書籍の方では`IN`も追加で指定されています。この違いはなんでしょう？