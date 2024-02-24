/*
 */
constexpr uint32_t num_icosa_vertices = 20 * 3 * 8;
constexpr uint32_t num_icosa_indices = 20 * 3;

/*
 */
static const float32_t icosa_vertices[num_icosa_vertices] = {
	-0.26287f,  0.42533f,  0.00000f,  0.00000f,  0.93417f, -0.35682f, 0.00000f, 0.00000f,
	 0.26287f,  0.42533f,  0.00000f,  0.00000f,  0.93417f, -0.35682f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f, -0.42533f,  0.00000f,  0.93417f, -0.35682f, 0.74118f, 1.00000f,
	 0.26287f,  0.42533f,  0.00000f,  0.00000f,  0.93417f,  0.35682f, 0.00000f, 0.00000f,
	-0.26287f,  0.42533f,  0.00000f,  0.00000f,  0.93417f,  0.35682f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f,  0.42533f,  0.00000f,  0.93417f,  0.35682f, 0.74118f, 1.00000f,
	-0.42533f,  0.00000f,  0.26287f, -0.35682f,  0.00000f,  0.93417f, 0.00000f, 0.00000f,
	 0.00000f, -0.26287f,  0.42533f, -0.35682f,  0.00000f,  0.93417f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f,  0.42533f, -0.35682f,  0.00000f,  0.93417f, 0.74118f, 1.00000f,
	 0.00000f, -0.26287f,  0.42533f,  0.35682f,  0.00000f,  0.93417f, 0.00000f, 0.00000f,
	 0.42533f,  0.00000f,  0.26287f,  0.35682f,  0.00000f,  0.93417f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f,  0.42533f,  0.35682f,  0.00000f,  0.93417f, 0.74118f, 1.00000f,
	 0.42533f,  0.00000f, -0.26287f,  0.35682f,  0.00000f, -0.93417f, 0.00000f, 0.00000f,
	 0.00000f, -0.26287f, -0.42533f,  0.35682f,  0.00000f, -0.93417f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f, -0.42533f,  0.35682f,  0.00000f, -0.93417f, 0.74118f, 1.00000f,
	 0.00000f, -0.26287f, -0.42533f, -0.35682f, -0.00000f, -0.93417f, 0.00000f, 0.00000f,
	-0.42533f,  0.00000f, -0.26287f, -0.35682f, -0.00000f, -0.93417f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f, -0.42533f, -0.35682f, -0.00000f, -0.93417f, 0.74118f, 1.00000f,
	-0.26287f, -0.42533f,  0.00000f,  0.00000f, -0.93417f,  0.35682f, 0.00000f, 0.00000f,
	 0.26287f, -0.42533f,  0.00000f,  0.00000f, -0.93417f,  0.35682f, 1.00000f, 0.25882f,
	 0.00000f, -0.26287f,  0.42533f,  0.00000f, -0.93417f,  0.35682f, 0.74118f, 1.00000f,
	 0.26287f, -0.42533f,  0.00000f,  0.00000f, -0.93417f, -0.35682f, 0.00000f, 0.00000f,
	-0.26287f, -0.42533f,  0.00000f,  0.00000f, -0.93417f, -0.35682f, 1.00000f, 0.25882f,
	 0.00000f, -0.26287f, -0.42533f,  0.00000f, -0.93417f, -0.35682f, 0.74118f, 1.00000f,
	-0.42533f,  0.00000f, -0.26287f, -0.93417f,  0.35682f,  0.00000f, 0.00000f, 0.00000f,
	-0.42533f,  0.00000f,  0.26287f, -0.93417f,  0.35682f,  0.00000f, 1.00000f, 0.25882f,
	-0.26287f,  0.42533f,  0.00000f, -0.93417f,  0.35682f,  0.00000f, 0.74118f, 1.00000f,
	-0.42533f,  0.00000f,  0.26287f, -0.93417f, -0.35682f,  0.00000f, 0.00000f, 0.00000f,
	-0.42533f,  0.00000f, -0.26287f, -0.93417f, -0.35682f,  0.00000f, 1.00000f, 0.25882f,
	-0.26287f, -0.42533f,  0.00000f, -0.93417f, -0.35682f,  0.00000f, 0.74118f, 1.00000f,
	 0.42533f,  0.00000f,  0.26287f,  0.93417f,  0.35682f,  0.00000f, 0.00000f, 0.00000f,
	 0.42533f,  0.00000f, -0.26287f,  0.93417f,  0.35682f,  0.00000f, 1.00000f, 0.25882f,
	 0.26287f,  0.42533f,  0.00000f,  0.93417f,  0.35682f,  0.00000f, 0.74118f, 1.00000f,
	 0.42533f,  0.00000f, -0.26287f,  0.93417f, -0.35682f,  0.00000f, 0.00000f, 0.00000f,
	 0.42533f,  0.00000f,  0.26287f,  0.93417f, -0.35682f,  0.00000f, 1.00000f, 0.25882f,
	 0.26287f, -0.42533f,  0.00000f,  0.93417f, -0.35682f,  0.00000f, 0.74118f, 1.00000f,
	-0.26287f,  0.42533f,  0.00000f, -0.57735f,  0.57735f,  0.57735f, 0.00000f, 0.00000f,
	-0.42533f,  0.00000f,  0.26287f, -0.57735f,  0.57735f,  0.57735f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f,  0.42533f, -0.57735f,  0.57735f,  0.57735f, 0.74118f, 1.00000f,
	 0.42533f,  0.00000f,  0.26287f,  0.57735f,  0.57735f,  0.57735f, 0.00000f, 0.00000f,
	 0.26287f,  0.42533f,  0.00000f,  0.57735f,  0.57735f,  0.57735f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f,  0.42533f,  0.57735f,  0.57735f,  0.57735f, 0.74118f, 1.00000f,
	-0.42533f,  0.00000f, -0.26287f, -0.57735f,  0.57735f, -0.57735f, 0.00000f, 0.00000f,
	-0.26287f,  0.42533f,  0.00000f, -0.57735f,  0.57735f, -0.57735f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f, -0.42533f, -0.57735f,  0.57735f, -0.57735f, 0.74118f, 1.00000f,
	 0.26287f,  0.42533f,  0.00000f,  0.57735f,  0.57735f, -0.57735f, 0.00000f, 0.00000f,
	 0.42533f,  0.00000f, -0.26287f,  0.57735f,  0.57735f, -0.57735f, 1.00000f, 0.25882f,
	 0.00000f,  0.26287f, -0.42533f,  0.57735f,  0.57735f, -0.57735f, 0.74118f, 1.00000f,
	-0.26287f, -0.42533f,  0.00000f, -0.57735f, -0.57735f, -0.57735f, 0.00000f, 0.00000f,
	-0.42533f,  0.00000f, -0.26287f, -0.57735f, -0.57735f, -0.57735f, 1.00000f, 0.25882f,
	 0.00000f, -0.26287f, -0.42533f, -0.57735f, -0.57735f, -0.57735f, 0.74118f, 1.00000f,
	 0.42533f,  0.00000f, -0.26287f,  0.57735f, -0.57735f, -0.57735f, 0.00000f, 0.00000f,
	 0.26287f, -0.42533f,  0.00000f,  0.57735f, -0.57735f, -0.57735f, 1.00000f, 0.25882f,
	 0.00000f, -0.26287f, -0.42533f,  0.57735f, -0.57735f, -0.57735f, 0.74118f, 1.00000f,
	-0.42533f,  0.00000f,  0.26287f, -0.57735f, -0.57735f,  0.57735f, 0.00000f, 0.00000f,
	-0.26287f, -0.42533f,  0.00000f, -0.57735f, -0.57735f,  0.57735f, 1.00000f, 0.25882f,
	 0.00000f, -0.26287f,  0.42533f, -0.57735f, -0.57735f,  0.57735f, 0.74118f, 1.00000f,
	 0.26287f, -0.42533f,  0.00000f,  0.57735f, -0.57735f,  0.57735f, 0.00000f, 0.00000f,
	 0.42533f,  0.00000f,  0.26287f,  0.57735f, -0.57735f,  0.57735f, 1.00000f, 0.25882f,
	 0.00000f, -0.26287f,  0.42533f,  0.57735f, -0.57735f,  0.57735f, 0.74118f, 1.00000f,
};

static const uint32_t icosa_indices[num_icosa_indices] = {
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
	12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
	36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
};
