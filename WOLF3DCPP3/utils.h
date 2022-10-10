#pragma once

#define M_PI   3.1415926
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

int rndIndex = 0;
const int rnd[256] = {887, 373, 472, 412, 312, 551, 647, 686, 682, 401, 569, 589, 46, 861, 845, 25, 968, 19, 324, 507, 693, 484, 547, 65, 421, 425, 786, 325, 320, 553, 870, 315,
610, 43, 100, 890, 935, 710, 550, 312, 474, 207, 728, 891, 349, 292, 430, 564, 573, 177, 547, 939, 590, 232, 97, 495, 941, 480, 749, 373, 855,
909, 898, 455, 348, 383, 671, 59, 755, 662, 226, 113, 285, 698, 407, 607, 116, 439, 188, 856, 478, 285, 774, 150, 507, 165, 408, 666, 167, 303, 592, 289, 890, 628, 682, 594,
809, 885, 214, 711, 528, 8, 463, 856, 813, 90, 266, 615, 170, 493, 190, 494, 670, 57, 565, 381, 355, 805, 388, 75, 464, 603, 204, 761, 459, 969, 689, 855, 808, 53, 301, 113,
325, 862, 511, 95, 821, 283, 638, 661, 369, 547, 559, 318, 658, 807, 141, 572, 459, 287, 260, 549, 485, 782, 516, 382, 897, 522, 382, 741, 87, 907, 394, 14, 776, 322, 815, 253,
932, 552, 425, 480, 302, 378, 776, 227, 365, 993, 771, 576, 402, 541, 286, 495, 45, 109, 635, 547, 189, 331, 918, 229, 493, 122, 357, 778, 656, 730, 232, 508, 873, 659, 543, 269,
309, 761, 777, 690, 729, 351, 263, 909, 147, 804, 925, 527, 531, 765, 369, 318, 39, 550, 754, 673, 937, 211, 854, 500, 373, 887, 863, 603, 815, 961, 591, 175, 442, 46, 184, 414,
28, 912, 230, 309, 29, 281, 732, 567, 750, 249, 869, 233, 244, 459, 900, 370};

int getRnd() { rndIndex++; if (rndIndex >= 256) { rndIndex = 0; } return rnd[rndIndex]; }
double FixAng(double a) { if (a > 359) { a -= 360; } if (a < 0) { a += 360; } return a; }
double distance(double ax, double ay, double bx, double by, double ang) { return cos(degToRad(ang)) * (bx - ax) - sin(degToRad(ang)) * (by - ay); }
int xyToMap(int x, int y, int mapX) { int mx = (int)(x) >> 6; int my = (int)(y) >> 6; return my * mapX + mx; }