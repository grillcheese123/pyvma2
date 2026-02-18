/* pyvma2 CFFI cdef — VMA 3.4.0 type and function declarations.
 * No preprocessor directives allowed (pure C declarations for CFFI).
 */

/* ===== Basic Vulkan types ===== */
typedef uint32_t VkFlags;
typedef uint32_t VkBool32;
typedef uint64_t VkDeviceSize;
typedef VkFlags  VkMemoryPropertyFlags;
typedef VkFlags  VkBufferUsageFlags;
typedef VkFlags  VkBufferCreateFlags;
typedef VkFlags  VkImageUsageFlags;
typedef VkFlags  VkImageCreateFlags;
typedef VkFlags  VkExternalMemoryHandleTypeFlagsKHR;

/* Opaque Vulkan handles — pointer-sized on all platforms */
typedef void* VkInstance;
typedef void* VkPhysicalDevice;
typedef void* VkDevice;
typedef void* VkBuffer;
typedef void* VkImage;
typedef void* VkDeviceMemory;
typedef void* VkCommandBuffer;

/* Vulkan function pointer types — opaque pointers passed through to VMA */
typedef void* PFN_vkGetInstanceProcAddr;
typedef void* PFN_vkGetDeviceProcAddr;
typedef void* PFN_vkGetPhysicalDeviceProperties;
typedef void* PFN_vkGetPhysicalDeviceMemoryProperties;
typedef void* PFN_vkAllocateMemory;
typedef void* PFN_vkFreeMemory;
typedef void* PFN_vkMapMemory;
typedef void* PFN_vkUnmapMemory;
typedef void* PFN_vkFlushMappedMemoryRanges;
typedef void* PFN_vkInvalidateMappedMemoryRanges;
typedef void* PFN_vkBindBufferMemory;
typedef void* PFN_vkBindImageMemory;
typedef void* PFN_vkGetBufferMemoryRequirements;
typedef void* PFN_vkGetImageMemoryRequirements;
typedef void* PFN_vkCreateBuffer;
typedef void* PFN_vkDestroyBuffer;
typedef void* PFN_vkCreateImage;
typedef void* PFN_vkDestroyImage;
typedef void* PFN_vkCmdCopyBuffer;
typedef void* PFN_vkGetBufferMemoryRequirements2KHR;
typedef void* PFN_vkGetImageMemoryRequirements2KHR;
typedef void* PFN_vkBindBufferMemory2KHR;
typedef void* PFN_vkBindImageMemory2KHR;
typedef void* PFN_vkGetPhysicalDeviceMemoryProperties2KHR;
typedef void* PFN_vkGetDeviceBufferMemoryRequirementsKHR;
typedef void* PFN_vkGetDeviceImageMemoryRequirementsKHR;
typedef void* PFN_vkGetMemoryWin32HandleKHR;

/* VMA callback function pointers — also opaque */
typedef void* PFN_vmaAllocateDeviceMemoryFunction;
typedef void* PFN_vmaFreeDeviceMemoryFunction;
typedef void* PFN_vmaCheckDefragmentationBreakFunction;

/* Vulkan allocation callbacks — always passed as NULL from Python */
typedef void* VkAllocationCallbacks;

/* VkBufferCreateInfo — matches Vulkan spec layout */
typedef struct VkBufferCreateInfo {
    uint32_t    sType;               /* VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO = 12 */
    const void* pNext;
    VkBufferCreateFlags flags;
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    uint32_t    sharingMode;         /* VkSharingMode enum value */
    uint32_t    queueFamilyIndexCount;
    const uint32_t* pQueueFamilyIndices;
} VkBufferCreateInfo;

/* VkImageCreateInfo — minimal for vmaCreateImage */
typedef struct VkImageCreateInfo {
    uint32_t    sType;               /* VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO = 14 */
    const void* pNext;
    VkImageCreateFlags flags;
    uint32_t    imageType;
    uint32_t    format;
    uint32_t    extent[3];           /* width, height, depth */
    uint32_t    mipLevels;
    uint32_t    arrayLayers;
    uint32_t    samples;
    uint32_t    tiling;
    VkImageUsageFlags usage;
    uint32_t    sharingMode;
    uint32_t    queueFamilyIndexCount;
    const uint32_t* pQueueFamilyIndices;
    uint32_t    initialLayout;
} VkImageCreateInfo;

/* VkMemoryRequirements */
typedef struct VkMemoryRequirements {
    VkDeviceSize size;
    VkDeviceSize alignment;
    uint32_t     memoryTypeBits;
} VkMemoryRequirements;

/* ===== VMA opaque handle types ===== */
typedef struct VmaAllocator_T*              VmaAllocator;
typedef struct VmaPool_T*                   VmaPool;
typedef struct VmaAllocation_T*             VmaAllocation;
typedef struct VmaDefragmentationContext_T* VmaDefragmentationContext;

/* ===== VMA structs ===== */

typedef struct VmaDeviceMemoryCallbacks {
    PFN_vmaAllocateDeviceMemoryFunction pfnAllocate;
    PFN_vmaFreeDeviceMemoryFunction     pfnFree;
    void*                               pUserData;
} VmaDeviceMemoryCallbacks;

typedef struct VmaVulkanFunctions {
    PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr                    vkGetDeviceProcAddr;
    PFN_vkGetPhysicalDeviceProperties          vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties    vkGetPhysicalDeviceMemoryProperties;
    PFN_vkAllocateMemory                       vkAllocateMemory;
    PFN_vkFreeMemory                           vkFreeMemory;
    PFN_vkMapMemory                            vkMapMemory;
    PFN_vkUnmapMemory                          vkUnmapMemory;
    PFN_vkFlushMappedMemoryRanges              vkFlushMappedMemoryRanges;
    PFN_vkInvalidateMappedMemoryRanges         vkInvalidateMappedMemoryRanges;
    PFN_vkBindBufferMemory                     vkBindBufferMemory;
    PFN_vkBindImageMemory                      vkBindImageMemory;
    PFN_vkGetBufferMemoryRequirements          vkGetBufferMemoryRequirements;
    PFN_vkGetImageMemoryRequirements           vkGetImageMemoryRequirements;
    PFN_vkCreateBuffer                         vkCreateBuffer;
    PFN_vkDestroyBuffer                        vkDestroyBuffer;
    PFN_vkCreateImage                          vkCreateImage;
    PFN_vkDestroyImage                         vkDestroyImage;
    PFN_vkCmdCopyBuffer                        vkCmdCopyBuffer;
    /* KHR dedicated allocation / Vulkan >= 1.1 */
    PFN_vkGetBufferMemoryRequirements2KHR      vkGetBufferMemoryRequirements2KHR;
    PFN_vkGetImageMemoryRequirements2KHR       vkGetImageMemoryRequirements2KHR;
    /* KHR bind_memory2 / Vulkan >= 1.1 */
    PFN_vkBindBufferMemory2KHR                 vkBindBufferMemory2KHR;
    PFN_vkBindImageMemory2KHR                  vkBindImageMemory2KHR;
    /* KHR memory budget / Vulkan >= 1.1 */
    PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR;
    /* KHR maintenance4 / Vulkan >= 1.3 */
    PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirements;
    PFN_vkGetDeviceImageMemoryRequirementsKHR  vkGetDeviceImageMemoryRequirements;
    /* Win32 external memory — void* placeholder */
    void* vkGetMemoryWin32HandleKHR;
} VmaVulkanFunctions;

typedef struct VmaAllocatorCreateInfo {
    uint32_t                            flags;
    VkPhysicalDevice                    physicalDevice;
    VkDevice                            device;
    VkDeviceSize                        preferredLargeHeapBlockSize;
    const VkAllocationCallbacks*        pAllocationCallbacks;
    const VmaDeviceMemoryCallbacks*     pDeviceMemoryCallbacks;
    const VkDeviceSize*                 pHeapSizeLimit;
    const VmaVulkanFunctions*           pVulkanFunctions;
    VkInstance                          instance;
    uint32_t                            vulkanApiVersion;
    /* VMA_EXTERNAL_MEMORY: per-memory-type external handle types */
    const VkExternalMemoryHandleTypeFlagsKHR* pTypeExternalMemoryHandleTypes;
} VmaAllocatorCreateInfo;

typedef struct VmaAllocatorInfo {
    VkInstance       instance;
    VkPhysicalDevice physicalDevice;
    VkDevice         device;
} VmaAllocatorInfo;

typedef struct VmaStatistics {
    uint32_t     blockCount;
    uint32_t     allocationCount;
    VkDeviceSize blockBytes;
    VkDeviceSize allocationBytes;
} VmaStatistics;

typedef struct VmaDetailedStatistics {
    VmaStatistics statistics;
    uint32_t      unusedRangeCount;
    VkDeviceSize  allocationSizeMin;
    VkDeviceSize  allocationSizeMax;
    VkDeviceSize  unusedRangeSizeMin;
    VkDeviceSize  unusedRangeSizeMax;
} VmaDetailedStatistics;

typedef struct VmaTotalStatistics {
    VmaDetailedStatistics memoryType[32];  /* VK_MAX_MEMORY_TYPES */
    VmaDetailedStatistics memoryHeap[16];  /* VK_MAX_MEMORY_HEAPS */
    VmaDetailedStatistics total;
} VmaTotalStatistics;

typedef struct VmaBudget {
    VmaStatistics statistics;
    VkDeviceSize  usage;
    VkDeviceSize  budget;
} VmaBudget;

typedef struct VmaAllocationCreateInfo {
    uint32_t              flags;
    uint32_t              usage;          /* VmaMemoryUsage enum */
    VkMemoryPropertyFlags requiredFlags;
    VkMemoryPropertyFlags preferredFlags;
    uint32_t              memoryTypeBits;
    VmaPool               pool;
    void*                 pUserData;
    float                 priority;
} VmaAllocationCreateInfo;

typedef struct VmaPoolCreateInfo {
    uint32_t     memoryTypeIndex;
    uint32_t     flags;
    VkDeviceSize blockSize;
    size_t       minBlockCount;
    size_t       maxBlockCount;
    float        priority;
    VkDeviceSize minAllocationAlignment;
    void*        pMemoryAllocateNext;
} VmaPoolCreateInfo;

typedef struct VmaAllocationInfo {
    uint32_t       memoryType;
    VkDeviceMemory deviceMemory;
    VkDeviceSize   offset;
    VkDeviceSize   size;
    void*          pMappedData;
    void*          pUserData;
    const char*    pName;
} VmaAllocationInfo;

typedef struct VmaAllocationInfo2 {
    VmaAllocationInfo allocationInfo;
    VkDeviceSize      blockSize;
    VkBool32          dedicatedMemory;
} VmaAllocationInfo2;

typedef struct VmaDefragmentationInfo {
    uint32_t     flags;
    VmaPool      pool;
    VkDeviceSize maxBytesPerPass;
    uint32_t     maxAllocationsPerPass;
    PFN_vmaCheckDefragmentationBreakFunction pfnBreakCallback;
    void*        pBreakCallbackUserData;
} VmaDefragmentationInfo;

typedef struct VmaDefragmentationMove {
    uint32_t       operation;
    VmaAllocation  srcAllocation;
    VmaAllocation  dstTmpAllocation;
} VmaDefragmentationMove;

typedef struct VmaDefragmentationPassMoveInfo {
    uint32_t               moveCount;
    VmaDefragmentationMove* pMoves;
} VmaDefragmentationPassMoveInfo;

typedef struct VmaDefragmentationStats {
    VkDeviceSize bytesMoved;
    VkDeviceSize bytesFreed;
    uint32_t     allocationsMoved;
    uint32_t     deviceMemoryBlocksFreed;
} VmaDefragmentationStats;

/* ===== VMA functions ===== */

/* Allocator lifecycle */
int vmaCreateAllocator(const VmaAllocatorCreateInfo* pCreateInfo, VmaAllocator* pAllocator);
void vmaDestroyAllocator(VmaAllocator allocator);
void vmaGetAllocatorInfo(VmaAllocator allocator, VmaAllocatorInfo* pAllocatorInfo);

/* Statistics */
void vmaCalculateStatistics(VmaAllocator allocator, VmaTotalStatistics* pStats);
void vmaGetHeapBudgets(VmaAllocator allocator, VmaBudget* pBudgets);

/* Memory type helpers */
int  vmaFindMemoryTypeIndex(VmaAllocator allocator, uint32_t memoryTypeBits,
         const VmaAllocationCreateInfo* pAllocationCreateInfo, uint32_t* pMemoryTypeIndex);
int  vmaFindMemoryTypeIndexForBufferInfo(VmaAllocator allocator,
         const VkBufferCreateInfo* pBufferCreateInfo,
         const VmaAllocationCreateInfo* pAllocationCreateInfo, uint32_t* pMemoryTypeIndex);
int  vmaFindMemoryTypeIndexForImageInfo(VmaAllocator allocator,
         const VkImageCreateInfo* pImageCreateInfo,
         const VmaAllocationCreateInfo* pAllocationCreateInfo, uint32_t* pMemoryTypeIndex);

/* Custom pools */
int  vmaCreatePool(VmaAllocator allocator, const VmaPoolCreateInfo* pCreateInfo, VmaPool* pPool);
void vmaDestroyPool(VmaAllocator allocator, VmaPool pool);
void vmaGetPoolStatistics(VmaAllocator allocator, VmaPool pool, VmaStatistics* pPoolStats);
void vmaCalculatePoolStatistics(VmaAllocator allocator, VmaPool pool, VmaDetailedStatistics* pPoolStats);
int  vmaCheckPoolCorruption(VmaAllocator allocator, VmaPool pool);
void vmaGetPoolName(VmaAllocator allocator, VmaPool pool, const char** ppName);
void vmaSetPoolName(VmaAllocator allocator, VmaPool pool, const char* pName);

/* Raw memory allocation */
int  vmaAllocateMemory(VmaAllocator allocator, const VkMemoryRequirements* pVkMemoryRequirements,
         const VmaAllocationCreateInfo* pCreateInfo, VmaAllocation* pAllocation, VmaAllocationInfo* pAllocationInfo);
int  vmaAllocateMemoryForBuffer(VmaAllocator allocator, VkBuffer buffer,
         const VmaAllocationCreateInfo* pCreateInfo, VmaAllocation* pAllocation, VmaAllocationInfo* pAllocationInfo);
int  vmaAllocateMemoryForImage(VmaAllocator allocator, VkImage image,
         const VmaAllocationCreateInfo* pCreateInfo, VmaAllocation* pAllocation, VmaAllocationInfo* pAllocationInfo);
void vmaFreeMemory(VmaAllocator allocator, VmaAllocation allocation);

/* Allocation info */
void vmaGetAllocationInfo(VmaAllocator allocator, VmaAllocation allocation, VmaAllocationInfo* pAllocationInfo);
void vmaGetAllocationInfo2(VmaAllocator allocator, VmaAllocation allocation, VmaAllocationInfo2* pAllocationInfo);
void vmaSetAllocationUserData(VmaAllocator allocator, VmaAllocation allocation, void* pUserData);
void vmaSetAllocationName(VmaAllocator allocator, VmaAllocation allocation, const char* pName);
void vmaGetAllocationMemoryProperties(VmaAllocator allocator, VmaAllocation allocation, VkMemoryPropertyFlags* pFlags);

/* Mapping */
int  vmaMapMemory(VmaAllocator allocator, VmaAllocation allocation, void** ppData);
void vmaUnmapMemory(VmaAllocator allocator, VmaAllocation allocation);
int  vmaFlushAllocation(VmaAllocator allocator, VmaAllocation allocation, VkDeviceSize offset, VkDeviceSize size);
int  vmaInvalidateAllocation(VmaAllocator allocator, VmaAllocation allocation, VkDeviceSize offset, VkDeviceSize size);
int  vmaFlushAllocations(VmaAllocator allocator, uint32_t allocationCount,
         const VmaAllocation* allocations, const VkDeviceSize* offsets, const VkDeviceSize* sizes);
int  vmaInvalidateAllocations(VmaAllocator allocator, uint32_t allocationCount,
         const VmaAllocation* allocations, const VkDeviceSize* offsets, const VkDeviceSize* sizes);

/* Convenience copy (VMA 3.1+) */
int  vmaCopyMemoryToAllocation(VmaAllocator allocator, const void* pSrcHostPointer,
         VmaAllocation dstAllocation, VkDeviceSize dstAllocationLocalOffset, VkDeviceSize size);
int  vmaCopyAllocationToMemory(VmaAllocator allocator, VmaAllocation srcAllocation,
         VkDeviceSize srcAllocationLocalOffset, void* pDstHostPointer, VkDeviceSize size);

/* Binding */
int  vmaBindBufferMemory(VmaAllocator allocator, VmaAllocation allocation, VkBuffer buffer);
int  vmaBindBufferMemory2(VmaAllocator allocator, VmaAllocation allocation,
         VkDeviceSize allocationLocalOffset, VkBuffer buffer, const void* pNext);
int  vmaBindImageMemory(VmaAllocator allocator, VmaAllocation allocation, VkImage image);
int  vmaBindImageMemory2(VmaAllocator allocator, VmaAllocation allocation,
         VkDeviceSize allocationLocalOffset, VkImage image, const void* pNext);

/* Buffer + Image convenience */
int  vmaCreateBuffer(VmaAllocator allocator, const VkBufferCreateInfo* pBufferCreateInfo,
         const VmaAllocationCreateInfo* pAllocationCreateInfo,
         VkBuffer* pBuffer, VmaAllocation* pAllocation, VmaAllocationInfo* pAllocationInfo);
int  vmaCreateBufferWithAlignment(VmaAllocator allocator, const VkBufferCreateInfo* pBufferCreateInfo,
         const VmaAllocationCreateInfo* pAllocationCreateInfo, VkDeviceSize minAlignment,
         VkBuffer* pBuffer, VmaAllocation* pAllocation, VmaAllocationInfo* pAllocationInfo);
void vmaDestroyBuffer(VmaAllocator allocator, VkBuffer buffer, VmaAllocation allocation);

int  vmaCreateImage(VmaAllocator allocator, const VkImageCreateInfo* pImageCreateInfo,
         const VmaAllocationCreateInfo* pAllocationCreateInfo,
         VkImage* pImage, VmaAllocation* pAllocation, VmaAllocationInfo* pAllocationInfo);
void vmaDestroyImage(VmaAllocator allocator, VkImage image, VmaAllocation allocation);

/* Defragmentation */
int  vmaBeginDefragmentation(VmaAllocator allocator, const VmaDefragmentationInfo* pInfo,
         VmaDefragmentationContext* pContext);
void vmaEndDefragmentation(VmaAllocator allocator, VmaDefragmentationContext context,
         VmaDefragmentationStats* pStats);
int  vmaBeginDefragmentationPass(VmaAllocator allocator, VmaDefragmentationContext context,
         VmaDefragmentationPassMoveInfo* pPassInfo);
int  vmaEndDefragmentationPass(VmaAllocator allocator, VmaDefragmentationContext context,
         VmaDefragmentationPassMoveInfo* pPassInfo);

/* Debug stats strings */
void vmaBuildStatsString(VmaAllocator allocator, char** ppStatsString, VkBool32 detailedMap);
void vmaFreeStatsString(VmaAllocator allocator, char* pStatsString);

/* Misc */
void vmaSetCurrentFrameIndex(VmaAllocator allocator, uint32_t frameIndex);
int  vmaCheckCorruption(VmaAllocator allocator, uint32_t memoryTypeBits);
