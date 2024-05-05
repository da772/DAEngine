
import { Vector2, Vector3, Vector4 } from "../vector";

/*
 struct FMaterialData
 {
   glm::vec4 baseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
   glm::vec2 uvScale = { 1.f,1.f };
   float metallicFactor = .5f;
   float roughnessFactor = .5f;
   float normalScale = 1.0f;
   float occlusionStrength = 1.0f;
   glm::vec3 emissiveFactor = { 0.0f, 0.0f, 0.0f };
 }
*/
export class MaterialData {
    baseColorFactor : Vector4 = new Vector4(1.0,1.0,1.0,1.0);
    emissiveFactor : Vector3 = new Vector3(0.0,0.0,0.0);
    uvScale : Vector2 = new Vector2(1.0, 1.0);
    metallicFactor : number = .5;
    roughnessFactor : number = .5;
    normalScale : number = 1.0;
    occlusionStrength : number = 1.0; 

    public constructor(
        baseColorFactor: Vector4,
        emissiveFactor: Vector3,
        uvScale: Vector2,
        metallicFactor: number,
        roughnessFactor: number,
        normalScale: number,
        occlusionStrength: number,
        baseColorTexture: string = "",
        normalTexture: string = "",
        metallicRoughnessTexture: string = "",
        emissiveTexture: string = "",
        occlusionTexture: string = ""
    ) {
        this.baseColorFactor = baseColorFactor;
        this.emissiveFactor = emissiveFactor;
        this.uvScale = uvScale;
        this.metallicFactor = metallicFactor;
        this.roughnessFactor = roughnessFactor;
        this.normalScale = normalScale;
        this.occlusionStrength = occlusionStrength;
        this.m_baseColorTexture = native_create_texture2d(baseColorTexture);
        this.m_emissiveTexture =  native_create_texture2d(emissiveTexture);
        this.m_occlusionTexture = native_create_texture2d(occlusionTexture);
        this.m_normalTexture = native_create_texture2d(normalTexture);
        this.m_metallicRoughnessTexture = native_create_texture2d(metallicRoughnessTexture);
    }

    public destroy() {
        native_destroy_texture2d(this.m_baseColorTexture);
        native_destroy_texture2d(this.m_emissiveTexture);
        native_destroy_texture2d(this.m_occlusionTexture);
        native_destroy_texture2d(this.m_normalTexture);
        native_destroy_texture2d(this.m_metallicRoughnessTexture);
    }

    private m_baseColorTexture : number;
	private m_emissiveTexture : number;
	private m_occlusionTexture : number;
	private m_normalTexture : number;
	private m_metallicRoughnessTexture : number;
}

declare function native_create_texture2d(path : string) : number;
declare function native_destroy_texture2d(ptr : number) : void;