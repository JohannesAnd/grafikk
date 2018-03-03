#pragma once

typedef struct Particle {
	float x;
	float y;
	float z;

	float dx;
	float dy;

	// Computed based on the frame's MVP, and is used for depth sorting.
	float depth;

	bool operator<(Particle& that) {
		// Sort in reverse order : far particles drawn first.
		return this->depth > that.depth;
	}
} Particle;