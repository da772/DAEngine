local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 2,["8"] = 2,["9"] = 2,["10"] = 2,["11"] = 16,["12"] = 16,["13"] = 16,["14"] = 26,["15"] = 33,["16"] = 33,["18"] = 34,["19"] = 34,["21"] = 35,["22"] = 35,["24"] = 36,["25"] = 36,["27"] = 37,["28"] = 37,["30"] = 17,["31"] = 17,["32"] = 17,["33"] = 17,["34"] = 17,["35"] = 17,["36"] = 17,["37"] = 18,["38"] = 19,["39"] = 20,["40"] = 21,["41"] = 22,["42"] = 23,["43"] = 39,["44"] = 40,["45"] = 41,["46"] = 42,["47"] = 43,["48"] = 44,["49"] = 45,["50"] = 46,["51"] = 47,["52"] = 48,["53"] = 49,["54"] = 50,["55"] = 51,["56"] = 25,["57"] = 54,["58"] = 55,["59"] = 56,["60"] = 57,["61"] = 58,["62"] = 59,["63"] = 54});
local ____exports = {}
local ____vector = require("daengine.vector")
local Vector2 = ____vector.Vector2
local Vector3 = ____vector.Vector3
local Vector4 = ____vector.Vector4
____exports.MaterialData = __TS__Class()
local MaterialData = ____exports.MaterialData
MaterialData.name = "MaterialData"
function MaterialData.prototype.____constructor(self, baseColorFactor, emissiveFactor, uvScale, metallicFactor, roughnessFactor, normalScale, occlusionStrength, baseColorTexture, normalTexture, metallicRoughnessTexture, emissiveTexture, occlusionTexture)
    if baseColorTexture == nil then
        baseColorTexture = ""
    end
    if normalTexture == nil then
        normalTexture = ""
    end
    if metallicRoughnessTexture == nil then
        metallicRoughnessTexture = ""
    end
    if emissiveTexture == nil then
        emissiveTexture = ""
    end
    if occlusionTexture == nil then
        occlusionTexture = ""
    end
    self.baseColorFactor = __TS__New(
        Vector4,
        1,
        1,
        1,
        1
    )
    self.emissiveFactor = __TS__New(Vector3, 0, 0, 0)
    self.uvScale = __TS__New(Vector2, 1, 1)
    self.metallicFactor = 0.5
    self.roughnessFactor = 0.5
    self.normalScale = 1
    self.occlusionStrength = 1
    self.baseColorFactor = baseColorFactor
    self.emissiveFactor = emissiveFactor
    self.uvScale = uvScale
    self.metallicFactor = metallicFactor
    self.roughnessFactor = roughnessFactor
    self.normalScale = normalScale
    self.occlusionStrength = occlusionStrength
    print("Creating MaterialData: " .. baseColorTexture)
    self.m_baseColorTexture = native_create_texture2d(nil, baseColorTexture)
    self.m_emissiveTexture = native_create_texture2d(nil, emissiveTexture)
    self.m_occlusionTexture = native_create_texture2d(nil, occlusionTexture)
    self.m_normalTexture = native_create_texture2d(nil, normalTexture)
    self.m_metallicRoughnessTexture = native_create_texture2d(nil, metallicRoughnessTexture)
end
function MaterialData.prototype.destroy(self)
    native_destroy_texture2d(nil, self.m_baseColorTexture)
    native_destroy_texture2d(nil, self.m_emissiveTexture)
    native_destroy_texture2d(nil, self.m_occlusionTexture)
    native_destroy_texture2d(nil, self.m_normalTexture)
    native_destroy_texture2d(nil, self.m_metallicRoughnessTexture)
end
return ____exports
