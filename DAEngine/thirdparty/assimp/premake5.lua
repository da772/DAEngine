project "assimp"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

  defines {
      -- "SWIG",
      "ASSIMP_BUILD_NO_OWN_ZLIB",

      "ASSIMP_BUILD_NO_X_IMPORTER",
      "ASSIMP_BUILD_NO_3DS_IMPORTER",
      "ASSIMP_BUILD_NO_MD3_IMPORTER",
      "ASSIMP_BUILD_NO_MDL_IMPORTER",
      "ASSIMP_BUILD_NO_MD2_IMPORTER",
      -- "ASSIMP_BUILD_NO_PLY_IMPORTER",
      "ASSIMP_BUILD_NO_ASE_IMPORTER",
      -- "ASSIMP_BUILD_NO_OBJ_IMPORTER",
      "ASSIMP_BUILD_NO_AMF_IMPORTER",
      "ASSIMP_BUILD_NO_HMP_IMPORTER",
      "ASSIMP_BUILD_NO_SMD_IMPORTER",
      "ASSIMP_BUILD_NO_MDC_IMPORTER",
      "ASSIMP_BUILD_NO_MD5_IMPORTER",
      "ASSIMP_BUILD_NO_STL_IMPORTER",
      "ASSIMP_BUILD_NO_LWO_IMPORTER",
      "ASSIMP_BUILD_NO_DXF_IMPORTER",
      "ASSIMP_BUILD_NO_NFF_IMPORTER",
      "ASSIMP_BUILD_NO_RAW_IMPORTER",
      "ASSIMP_BUILD_NO_OFF_IMPORTER",
      "ASSIMP_BUILD_NO_AC_IMPORTER",
      "ASSIMP_BUILD_NO_BVH_IMPORTER",
      "ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
      "ASSIMP_BUILD_NO_IRR_IMPORTER",
      "ASSIMP_BUILD_NO_Q3D_IMPORTER",
      "ASSIMP_BUILD_NO_B3D_IMPORTER",
      -- "ASSIMP_BUILD_NO_COLLADA_IMPORTER",
      "ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
      "ASSIMP_BUILD_NO_CSM_IMPORTER",
      "ASSIMP_BUILD_NO_3D_IMPORTER",
      "ASSIMP_BUILD_NO_LWS_IMPORTER",
      "ASSIMP_BUILD_NO_OGRE_IMPORTER",
      "ASSIMP_BUILD_NO_OPENGEX_IMPORTER",
      "ASSIMP_BUILD_NO_MS3D_IMPORTER",
      "ASSIMP_BUILD_NO_COB_IMPORTER",
      "ASSIMP_BUILD_NO_BLEND_IMPORTER",
      "ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
      "ASSIMP_BUILD_NO_NDO_IMPORTER",
      "ASSIMP_BUILD_NO_IFC_IMPORTER",
      "ASSIMP_BUILD_NO_XGL_IMPORTER",
      --"ASSIMP_BUILD_NO_FBX_IMPORTER",
      "ASSIMP_BUILD_NO_ASSBIN_IMPORTER",
      --"ASSIMP_BUILD_NO_GLTF_IMPORTER",
      "ASSIMP_BUILD_NO_C4D_IMPORTER",
      "ASSIMP_BUILD_NO_3MF_IMPORTER",
      "ASSIMP_BUILD_NO_X3D_IMPORTER",
      "ASSIMP_BUILD_NO_MMD_IMPORTER",
      
      "ASSIMP_BUILD_NO_STEP_EXPORTER",
      "ASSIMP_BUILD_NO_SIB_IMPORTER",

      -- "ASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS",
      -- "ASSIMP_BUILD_NO_FLIPUVS_PROCESS",
      -- "ASSIMP_BUILD_NO_FLIPWINDINGORDER_PROCESS",
      -- "ASSIMP_BUILD_NO_CALCTANGENTS_PROCESS",
      "ASSIMP_BUILD_NO_JOINVERTICES_PROCESS",
      -- "ASSIMP_BUILD_NO_TRIANGULATE_PROCESS",
      "ASSIMP_BUILD_NO_GENFACENORMALS_PROCESS",
      -- "ASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS",
      "ASSIMP_BUILD_NO_REMOVEVC_PROCESS",
      "ASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS",
      "ASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS",
      "ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS",
      -- "ASSIMP_BUILD_NO_VALIDATEDS_PROCESS",
      "ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS",
      "ASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS",
      "ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS",
      "ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS",
      "ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS",
      "ASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS",
      "ASSIMP_BUILD_NO_GENUVCOORDS_PROCESS",
      "ASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS",
      "ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS",
      "ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS",
      "ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS",
      "ASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS",
      "ASSIMP_BUILD_NO_DEBONE_PROCESS",
      "ASSIMP_BUILD_NO_EMBEDTEXTURES_PROCESS",
      "ASSIMP_BUILD_NO_GLOBALSCALE_PROCESS",
  }

  files {
      "include/**",
      "src/Assimp.cpp",
      "src/BaseImporter.cpp",
      "src/ColladaLoader.cpp",
      "src/ColladaParser.cpp",
      "src/CreateAnimMesh.cpp",
      "src/PlyParser.cpp",
      "src/PlyLoader.cpp",
      "src/BaseProcess.cpp",
      "src/EmbedTexturesProcess.cpp",
      "src/ConvertToLHProcess.cpp",
      "src/DefaultIOStream.cpp",
      "src/DefaultIOSystem.cpp",
      "src/DefaultLogger.cpp",
      "src/GenVertexNormalsProcess.cpp",
      "src/Importer.cpp",
      "src/ImporterRegistry.cpp",
      "src/MaterialSystem.cpp",
      "src/PostStepRegistry.cpp",
      "src/ProcessHelper.cpp",
      "src/scene.cpp",
      "src/ScenePreprocessor.cpp",
      "src/ScaleProcess.cpp",
      "src/SGSpatialSort.cpp",
      "src/SkeletonMeshBuilder.cpp",
      "src/SpatialSort.cpp",
      "src/TriangulateProcess.cpp",
      "src/ValidateDataStructure.cpp",
      "src/Version.cpp",
      "src/VertexTriangleAdjacency.cpp",
      "src/ObjFileImporter.cpp",
      "src/ObjFileMtlImporter.cpp",
      "src/ObjFileParser.cpp",
      "src/glTFImporter.cpp",
      "src/glTF2Importer.cpp",
      "src/MakeVerboseFormat.cpp",
      "src/CalcTangentsProcess.cpp",
      "src/ScaleProcess.cpp",
      "src/EmbedTexturesProcess.cpp",
      "contrib/irrXML/*",
      -- fbx
      "src/FBXAnimation.cpp",
      "src/FBXBinaryTokenizer.cpp",
      "src/FBXCommon.h",
      "src/FBXCompileConfig.h",
      "src/FBXConverter.cpp",
      "src/FBXConverter.h",
      "src/FBXDeformer.cpp",
      "src/FBXDocument.cpp",
      "src/FBXDocument.h",
      "src/FBXDocumentUtil.cpp",
      "src/FBXDocumentUtil.h",
      "src/FBXExporter.cpp",
      "src/FBXExporter.h",
      "src/FBXExportNode.cpp",
      "src/FBXExportNode.h",
      "src/FBXExportProperty.cpp",
      "src/FBXExportProperty.h",
      "src/FBXImporter.cpp",
      "src/FBXImporter.h",
      "src/FBXImportSettings.h",
      "src/FBXMaterial.cpp",
      "src/FBXMeshGeometry.cpp",
      "src/FBXMeshGeometry.h",
      "src/FBXModel.cpp",
      "src/FBXNodeAttribute.cpp",
      "src/FBXParser.cpp",
      "src/FBXParser.h",
      "src/FBXProperties.h",
      "src/FBXProperties.cpp",
      "src/FBXTokenizer.cpp",
      "src/FBXTokenizer.h",
      "src/FBXUtil.h",
      "src/FBXUtil.cpp",
  }

  includedirs {
      "include",
      "contrib/irrXML",
      "contrib/zlib",
      "contrib/rapidjson/include",
  }

  links {
        "zlib"
  }

  filter "system:macosx"
        xcodebuildsettings
        { 
            ["ALWAYS_SEARCH_USER_PATHS"] = "YES",
            ["SKIP_INSTALL"] = "YES"
        }
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
			
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:android"
        cppdialect "gnu++17"
        linkoptions { "-lm" }
        exceptionhandling ("On")
        rtti ("On")
        androidapilevel(android_version)
        
        filter "platforms:x86"
            architecture "x86"
        filter "platforms:ARM"
            architecture "ARM"
        filter "platforms:ARM64"
            architecture "ARM64"
        filter "platforms:x64"
			architecture "x64"


        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:ios"
		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
            ["ARCHS"] = "$(ARCHS_STANDARD)",
            ["SKIP_INSTALL"] = "YES"
        }
        
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:windows"
        systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:linux"
        systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
