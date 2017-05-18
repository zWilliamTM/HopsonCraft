#include "Biome.h"

#include "../../Util/STD_Util.h"

using RNG = Random::Generator<std::minstd_rand>;

Biome::Biome(std::string&& fileName, const std::string& worldGen)
:   m_fileName      (std::move(fileName))
,   m_worldGenName  (worldGen)
{
    std::string fullName = "Data/WorldGen/" + m_worldGenName + "/" + m_fileName + ".biome";
    Loader::load(fullName);
}

CBlock Biome::getSurfaceBlock(RNG& rd)
{
    return getBlock(m_surfaceBlocks, rd);
}

CBlock Biome::getFloraBlock(RNG& rd)
{
    return getBlock(m_floraBlocks, rd);
}

Structure_ID Biome::getTree(RNG& rd)
{
    return m_trees.holder[rd.intInRange(0, m_trees.total)];
}

CBlock Biome::getBlock(const Biome::Gen_Container<CBlock>& container, RNG rd)
{
    return container.holder[rd.intInRange(0, container.total)];
}

const Noise::Data& Biome::getNoise() const
{
    return m_noise;
}

bool Biome::parseLine(const std::string& line, std::ifstream& inFile)
{
    if (areStringsSame(line, "Noise"))
    {
        loadNoise(inFile);
    }
    else if (areStringsSame(line, "Surface"))
    {
        loadIntoBlockList(inFile, m_surfaceBlocks);
    }
    else if (areStringsSame(line, "Flora"))
    {
        loadIntoBlockList(inFile, m_floraBlocks);
    }
    else if (areStringsSame(line, "Tree"))
    {
        loadTree(inFile);
    }
    else
    {
        return false;
    }
    return true;
}

void Biome::loadNoise(std::ifstream& inFile)
{
    int octaves,
        amplitude,
        heightOffset;

    float   roughness,
            smoothness;


    inFile >> octaves >> amplitude >> roughness >> smoothness >> heightOffset;

    m_noise = {octaves, amplitude, roughness, smoothness, heightOffset};
}

void Biome::loadTree(std::ifstream& inFile)
{
    int id, freq;
    inFile >> id >> freq;

    auto idEnum = (Structure_ID)id;

    for (int i = 0; i <freq; i++)
    {
        m_trees.holder.push_back(idEnum);
    }
    m_trees.total += freq;
}

void Biome::loadIntoBlockList(std::ifstream& inFile,
                              Gen_Container<CBlock>& container)
{
    int id, freq;
    inFile >> id >> freq;

    for (int i = 0; i <freq; i++)
    {
        container.holder.emplace_back(id);
    }
    container.total += freq;
}








