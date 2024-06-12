#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <cassert>
#include<Vector3.h>

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック

};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {





public:

void ResetMapChipData();

void LoadMapChipCsv(const std::string& filepath);

MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

static inline const uint32_t GetNumBlockVirtical()  { return kNumBlockVirtical; }
static inline const uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }

private:
// 1ブロックのサイズ
static inline const float kBlockWindth = 1.0f;
static inline const float kBlockHeight = 1.0f;
// ブロックの個数
static inline const uint32_t kNumBlockVirtical = 20;
static inline const uint32_t kNumBlockHorizontal = 100;

MapChipData mapChipData_;
};

