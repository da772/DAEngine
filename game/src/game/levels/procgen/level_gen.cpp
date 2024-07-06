// header
#include "level_gen.h"


std::vector<FLocation> CLevelGenerator::generateTargets(uint32_t amt)
{
	std::vector<FLocation> unreached;

	srand(m_settings.Seed);

	unreached.push_back(m_settings.StartingNode);

	for (int i = 0; i < amt; i++) {
		int x = rand() % m_settings.Width;
		int y = rand() % m_settings.Height;;

		unreached.push_back({ x,y });
	}
	return unreached;
}


void CLevelGenerator::generate(const std::vector<FLocation>& path)
{
	generateInternal(path);
}

void CLevelGenerator::generate()
{
	generateInternal(generateTargets(m_settings.Nodes));
}

void CLevelGenerator::generateInternal(std::vector<FLocation> unreached)
{
	std::vector<FLocation> reached = { unreached[0] };
	unreached.erase(unreached.begin());

	while (!unreached.empty())
	{
		int max = std::numeric_limits<int>::max();
		int rIndex = 0;
		int uIndex = 0;

		for (int i = 0; i < reached.size(); i++) {
			for (int j = 0; j < unreached.size(); j++) {
				FLocation v1 = reached[i];
				FLocation v2 = unreached[j];

				int dist = FLocation::distanceSq(v1, v2);

				if (dist < max) {
					max = dist;
					rIndex = i;
					uIndex = j;
				}
			}
		}

		m_path.push_back({ reached[rIndex], unreached[uIndex] });
		reached.push_back(unreached[uIndex]);
		unreached.erase(unreached.begin() + uIndex);
	}
}


