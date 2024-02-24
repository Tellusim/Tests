/*
 */
struct GeoSphere {
	
	uint32_t num_vertices = 0;
	uint32_t num_indices = 0;
	float32_t *vertices = nullptr;
	uint32_t *indices = nullptr;
	
	GeoSphere(uint32_t depth, float32_t *vertices, uint32_t *indices) : vertices(vertices), indices(indices) {
		static const float32_t octa_vertices[10 * 5] = {
			 0.00000f,  0.50000f,  0.00000f, 0.00000f, 0.00000f,
			-0.35355f,  0.00000f, -0.35355f, 1.00000f, 0.25882f,
			-0.35355f,  0.00000f,  0.35355f, 0.74118f, 1.00000f,
			 0.35355f,  0.00000f, -0.35355f, 1.00000f, 0.25882f,
			-0.35355f,  0.00000f, -0.35355f, 0.74118f, 1.00000f,
			 0.35355f,  0.00000f,  0.35355f, 1.00000f, 0.25882f,
			 0.35355f,  0.00000f, -0.35355f, 0.74118f, 1.00000f,
			-0.35355f,  0.00000f,  0.35355f, 1.00000f, 0.25882f,
			 0.35355f,  0.00000f,  0.35355f, 0.74118f, 1.00000f,
			 0.00000f, -0.50000f,  0.00000f, 0.00000f, 0.00000f,
		};
		static const uint32_t octa_indices[8 * 3] = {
			0, 1, 2, 0, 3, 4, 0, 5, 6, 0, 7, 8,
			9, 1, 6, 9, 7, 4, 9, 3, 8, 9, 5, 2,
		};
		for(uint32_t i = 0, j = 0; i < 8; i++, j += 3) {
			uint32_t i0 = Maxu32, i1 = Maxu32, i2 = Maxu32;
			const float32_t *v0 = octa_vertices + octa_indices[j + 0] * 5;
			const float32_t *v1 = octa_vertices + octa_indices[j + 1] * 5;
			const float32_t *v2 = octa_vertices + octa_indices[j + 2] * 5;
			subdivide(v0, v1, v2, i0, i1, i2, depth);
		}
	}
	
	uint32_t add_vertex(const float32_t *v) {
		float32_t *vertex = vertices + num_vertices * 8;
		float32_t length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		float32_t ilength = 1.0f / Tellusim::sqrt(length);
		vertex[0] = v[0] * ilength * 0.5f;
		vertex[1] = v[1] * ilength * 0.5f;
		vertex[2] = v[2] * ilength * 0.5f;
		vertex[3] = v[0] * ilength;
		vertex[4] = v[1] * ilength;
		vertex[5] = v[2] * ilength;
		vertex[6] = v[3];
		vertex[7] = v[4];
		return num_vertices++;
	}
	
	void subdivide(const float32_t *v0, const float32_t *v1, const float32_t *v2, uint32_t &i0, uint32_t &i1, uint32_t &i2, uint32_t depth) {
		if(depth == 0) {
			if(i0 == Maxu32) i0 = add_vertex(v0);
			if(i1 == Maxu32) i1 = add_vertex(v1);
			if(i2 == Maxu32) i2 = add_vertex(v2);
			indices[num_indices++] = i0;
			indices[num_indices++] = i1;
			indices[num_indices++] = i2;
		} else {
			float32_t v01[5], v12[5], v20[5];
			for(uint32_t i = 0; i < 5; i++) {
				v01[i] = (v0[i] + v1[i]) * 0.5f;
				v12[i] = (v1[i] + v2[i]) * 0.5f;
				v20[i] = (v2[i] + v0[i]) * 0.5f;
			}
			uint32_t i01 = Maxu32, i12 = Maxu32, i20 = Maxu32;
			subdivide(v0, v01, v20, i0, i01, i20, depth - 1);
			subdivide(v1, v12, v01, i1, i12, i01, depth - 1);
			subdivide(v2, v20, v12, i2, i20, i12, depth - 1);
			subdivide(v01, v12, v20, i01, i12, i20, depth - 1);
		}
	}
};

/*
 */
constexpr uint32_t sphere_depth = 3;
constexpr uint32_t sphere_multiplier = 1 << sphere_depth * 2;
constexpr uint32_t num_sphere_vertices = sphere_multiplier * 8 * 8 + 2 * 8 * 8;
constexpr uint32_t num_sphere_indices = sphere_multiplier * 8 * 3;

static float32_t sphere_vertices[num_sphere_vertices];
static uint32_t sphere_indices[num_sphere_vertices];

static GeoSphere geo_sphere(sphere_depth, sphere_vertices, sphere_indices);

TS_ASSERT(geo_sphere.num_vertices * 8 == num_sphere_vertices);
TS_ASSERT(geo_sphere.num_indices == num_sphere_indices);
