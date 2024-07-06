#pragma once
#include <daengine.h>

struct FLocation
{
	int X, Y;

	static int distanceSq(FLocation v1, FLocation v2) {
		return ((v1.X - v2.X) * (v1.X - v2.X)) * ((v1.Y - v2.Y) * (v1.Y - v2.Y));
	}

};


struct FLevelGenSettings
{
	uint32_t Seed;
	uint32_t Nodes;
	FLocation StartingNode;
	uint32_t Width, Height;
};


struct FPath {
	FLocation Start, End;
};

class CLevelGenerator
{
public:
	inline CLevelGenerator() {};
	inline CLevelGenerator(const FLevelGenSettings& settings) : m_settings(settings) { };

	inline const std::vector<FPath>& getPath() const { return m_path; }

	void generate();
	void generate(const std::vector<FLocation>& path);
private:
	void generateInternal(std::vector<FLocation> unreached);
	std::vector<FLocation> generateTargets(uint32_t amt);

private:
	std::vector<FPath> m_path;
	FLevelGenSettings m_settings;
};