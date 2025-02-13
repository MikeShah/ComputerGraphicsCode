/// @file transform.d
// The purpose of this type is to store
// transformations of 3D entities (cameras, objects, etc.)
struct Transform{
public:

    // Constructor for a new transform
    this(mat4 m){
			mModelMatrix = m;
		}
    // Destructor for the transform
    ~this(){}
    // Resets matrix transformations to the identity matrix
    void LoadIdentity();
    // Series if helper functions for modifying the
		// Transform
    void Translate(float x, float y, float z);
    void Rotate(float radians, float x, float y, float z);
    void Scale(float x, float y, float z);
		// Handle general transformations with binary operators
		Transform opBinary(string op)(Transform t){
			// ... Handle +,-,*
		}
		// Apply Transformation (Same as 'multiplication')
    void ApplyTransform(Transform t);

    // Returns the transformation matrix
    mat4 GetInternalMatrix(){
			return mModelMatrix;
		}

private:
    // Stores the actual transformation matrix
    mat4 mModelMatrix;
}
