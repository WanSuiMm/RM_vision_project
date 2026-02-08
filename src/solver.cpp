#include "solver.hpp"

void PoseSolver::solve(ArmorPlate& armor) {
    float pixel_height = (armor.left.length + armor.right.length) / 2.0f;
    armor.z_3d = (CAM_FY * REAL_ARMOR_HEIGHT) / pixel_height;
    armor.x_3d = (armor.center.x - CAM_CX) * armor.z_3d / CAM_FX;
    armor.y_3d = (armor.center.y - CAM_CY) * armor.z_3d / CAM_FY;
}