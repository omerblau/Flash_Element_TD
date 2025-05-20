//bagel_cfg.h file
#pragma once

constexpr Bagel Params{
    .DynamicResize = false,
    .IdBagSize = 16, // number of returned IDs to cache
    .InitialEntities = 64, // total entities your world can hold
    .InitialPackedSize = 32, // initial capacity of packed component arrays

    // we've got more than 10 different component types:
    .MaxComponents = 32
};

//BAGEL_STORAGE(Position,PackedStorage)
