#pragma once

struct aiNodeAnim;

namespace da::graphics {

    struct FKeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    struct FKeyRotation
    {
        glm::quat orientation;
        float timeStamp;
    };

    struct FKeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

    class CAnimatedBone
    {
    public:

        CAnimatedBone(CHashString name, int ID, const aiNodeAnim* channel);
        inline CAnimatedBone() {};

        /*interpolates  b/w positions,rotations & scaling keys based on the curren time of
        the animation and prepares the local transformation matrix by combining all keys
        tranformations*/
        void Update(float animationTime);

        inline glm::mat4 GetLocalTransform() const { return m_LocalTransform; }
        inline CHashString GetBoneName() const { return m_Name; }
        inline int GetBoneID() const { return m_ID; }


        /* Gets the current index on mKeyPositions to interpolate to based on
        the current animation time*/
        int GetPositionIndex(float animationTime);

        /* Gets the current index on mKeyRotations to interpolate to based on the
        current animation time*/
        int GetRotationIndex(float animationTime);

        /* Gets the current index on mKeyScalings to interpolate to based on the
        current animation time */
        int GetScaleIndex(float animationTime);

	private:
		std::vector<FKeyPosition> m_Positions;
		std::vector<FKeyRotation> m_Rotations;
		std::vector<FKeyScale> m_Scales;
		int m_NumPositions;
		int m_NumRotations;
		int m_NumScalings;

        float m_lastUpdateTime = -1.f;
		glm::mat4 m_LocalTransform;
		CHashString m_Name;
		int m_ID;


    private:

        /* Gets normalized value for Lerp & Slerp*/
        float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

        /*figures out which position keys to interpolate b/w and performs the interpolation
        and returns the translation matrix*/
        glm::mat4 InterpolatePosition(float animationTime);

        /*figures out which rotations keys to interpolate b/w and performs the interpolation
        and returns the rotation matrix*/
        glm::mat4 InterpolateRotation(float animationTime);

        /*figures out which scaling keys to interpolate b/w and performs the interpolation
        and returns the scale matrix*/
        glm::mat4 InterpolateScaling(float animationTime);

    };
}