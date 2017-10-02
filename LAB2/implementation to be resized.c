#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

// GLOBAL VARIABLES
/***********************************************************************************************************************/
// width and height of this image, globally visible
unsigned int g_width, g_height;
// global index for double-buffering
bool g_rendered = 0;
// global tiling blocksize
const int T = 1024;
// global tmps
int temp = 0;
int temp1 = 0;
int temp2 = 0;
int temp3 = 0;
int temp4 = 0;
/**********************************************************************************************************************/

// FUNCTION DECLARATIONS
/***********************************************************************************************************************/
void processMoveUp(unsigned char *buffer_frame, unsigned char *rendered_frame, int offset);
void processMoveDown(unsigned char *buffer_frame, unsigned char *rendered_frame, int offset);
void processMoveLeft(unsigned char *buffer_frame, unsigned char *rendered_frame, int offset);
void processMoveRight(unsigned char *buffer_frame, unsigned char *rendered_frame, int offset);
void processRotateCCW(unsigned char *buffer_frame, unsigned char *rendered_frame, int rotate_iteration);
void processRotateCW(unsigned char *buffer_frame, unsigned char *rendered_frame, int rotate_iteration);
void processMirrorX(unsigned char *buffer_frame, unsigned char *rendered_frame, int _unused);
void processMirrorY(unsigned char *buffer_frame, unsigned char *rendered_frame, int _unused);
// helper functions
//----------------------------------------------------------------------------------------------------------------------
void rotateCW90(unsigned char *buffer_frame, unsigned char *rendered_frame);
void rotateCCW90(unsigned char *buffer_frame, unsigned char *rendered_frame);
void rotateCW180(unsigned char *buffer_frame, unsigned char *rendered_frame);
#ifndef min
    #define min(a,b) ((a) < (b) ? (a) : (b))
#endif
//----------------------------------------------------------------------------------------------------------------------
/***********************************************************************************************************************/

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveUp(unsigned char *buffer_frame, unsigned char *rendered_frame, int offset) {
    // handle negative offsets
    if (offset < 0){
        processMoveDown(buffer_frame, rendered_frame, -offset);
        return;
    }
	
    temp = g_width * 3;
    temp2 = temp * g_height;
    temp4 = temp * offset;
    memset(rendered_frame, 255, temp2);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < (g_height - offset); row+=T) {
        for (int column = 0; column < g_width; column+=T) {
            for (int row_t = row; row_t < min(row + T, g_height - offset); row_t++) {
                temp1 = temp * row_t;
                temp2 = temp1 + temp4;
                for (int column_t = column; column_t < min(column + T, g_width); column_t++) {
                    temp3 = column_t * 3;
                    int position_rendered_frame = temp1 + temp3;
                    int position_buffer_frame = temp2 + temp3;

                    if (buffer_frame[position_buffer_frame] == 255 &&
                        buffer_frame[position_buffer_frame + 1] == 255 &&
                        buffer_frame[position_buffer_frame + 2] == 255) continue;

                    rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
                    rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
                    rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
                }
            }
        }
    }

    g_rendered = !g_rendered;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image left
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveRight(unsigned char *buffer_frame, unsigned char *rendered_frame, int offset) {
	// handle negative offsets
    if (offset < 0){
        processMoveLeft(buffer_frame, rendered_frame, -offset);
        return;
    }

    temp = g_width * 3;
    temp2 = g_height * temp;
    temp4 = 3 * offset;
    memset(rendered_frame, 255, temp2);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < g_height; row+=T) {
        for (int column = offset; column < g_width; column+=T) {
            for (int row_t = row; row_t < min(row + T, g_height); row_t++) {
                temp1 = temp * row_t;
                temp2 = temp1 - temp4;
                for (int column_t = column; column_t < min(column + T, g_width); column_t++) {
                    temp3 = column_t * 3;
                    int position_rendered_frame = temp1 + temp3;
                    int position_buffer_frame = temp2 + temp3;

                    if (buffer_frame[position_buffer_frame] == 255 &&
                        buffer_frame[position_buffer_frame + 1] == 255 &&
                        buffer_frame[position_buffer_frame + 2] == 255) continue;

                    rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
                    rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
                    rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
                }
            }
        }
    }

    g_rendered = !g_rendered;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveDown(unsigned char *buffer_frame, unsigned char *rendered_frame, int offset) {
	// handle negative offsets
    if (offset < 0){
        processMoveUp(buffer_frame, rendered_frame, -offset);
        return;
    }

    temp = g_width * 3;
    temp2 = temp * g_height;
    temp4 = offset * temp;
    memset(rendered_frame, 255, temp2);

    // store shifted pixels to temporary buffer
    for (int row = offset; row < g_height; row+=T) {
        for (int column = 0; column < g_width; column+=T) {
            for (int row_t = row; row_t < min(row + T, g_height); row_t++) {
                temp1 = temp * row_t;
                temp2 = temp1 - temp4;
                for (int column_t = column; column_t < min(column + T, g_width); column_t++) {
                    temp3 = column_t * 3;
                    int position_rendered_frame = temp1 + temp3;
                    int position_buffer_frame = temp2 + temp3;

                    if (buffer_frame[position_buffer_frame] == 255 &&
                        buffer_frame[position_buffer_frame + 1] == 255 &&
                        buffer_frame[position_buffer_frame + 2] == 255) continue;

                    rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
                    rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
                    rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
                }
            }
        }
    }

    g_rendered = !g_rendered;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
void processMoveLeft(unsigned char *buffer_frame, unsigned char *rendered_frame, int offset) {
    // handle negative offsets
    if (offset < 0){
        processMoveRight(buffer_frame, rendered_frame, -offset);
        return;
    }

    temp = g_width * 3;
    temp2 = temp * g_height;
    temp4 = 3 * offset;
    memset(rendered_frame, 255, temp2);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < g_height; row+=T) {
        for (int column = 0; column < (g_width - offset); column+=T) {
            for (int row_t = row; row_t < min(row + T, g_height); row_t++) {
                temp1 = temp * row_t;
                temp2 = temp1 + temp4;
                for (int column_t = column; column_t < min(column + T, g_width - offset); column_t++) {
                    temp3 = column_t * 3;
                    int position_rendered_frame = temp1 + temp3;
                    int position_buffer_frame = temp2 + temp3;

                    if (buffer_frame[position_buffer_frame] == 255 &&
                        buffer_frame[position_buffer_frame + 1] == 255 &&
                        buffer_frame[position_buffer_frame + 2] == 255) continue;

                    rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
                    rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
                    rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
                }
            }
        }
    }

    g_rendered = !g_rendered;
}

void rotateCW90(unsigned char *buffer_frame, unsigned char *rendered_frame) {
    // store shifted pixels to temporary buffer
    int render_column = g_width - 1;
    int render_row = 0;
    temp = g_width * 3;
    for (int row = 0; row < g_width; row+=T) {
        for (int column = 0; column < g_height; column+=T) {
            for (int row_t = row; row_t < min(row + T, g_width); row_t++) {
                temp2 = render_column * 3;
                temp4 = row_t * temp;
                for (int column_t = column; column_t < min(column + T, g_height); column_t++) {
                    temp1 = render_row * temp + temp2;
                    int position_frame_buffer = temp4 + column_t * 3;
                    rendered_frame[temp1] = buffer_frame[position_frame_buffer];
                    rendered_frame[temp1 + 1] = buffer_frame[position_frame_buffer + 1];
                    rendered_frame[temp1 + 2] = buffer_frame[position_frame_buffer + 2];
                    render_row += 1;
                }
                render_row = 0;
                render_column -= 1;
            }
        }
    }

    g_rendered = !g_rendered;
}

void rotateCCW90(unsigned char *buffer_frame, unsigned char *rendered_frame) {
    int render_column = 0;
    int render_row = g_height - 1;
    temp = g_width * 3;
    for (int row = 0; row < g_width; row+=T) {
        for (int column = 0; column < g_height; column+=T) {
            for (int row_t = row; row_t < min(row + T, g_width); row_t++) {
                temp2 = render_column * 3;
                temp4 = row_t * temp;
                for (int column_t = column; column_t < min(column + T, g_height); column_t++) {
                    temp1 = render_row * temp + temp2;
                    int position_frame_buffer = temp4 + column_t * 3;
                    rendered_frame[temp1] = buffer_frame[position_frame_buffer];
                    rendered_frame[temp1 + 1] = buffer_frame[position_frame_buffer + 1];
                    rendered_frame[temp1 + 2] = buffer_frame[position_frame_buffer + 2];
                    render_row -= 1;
                }
                render_row = g_height - 1;
                render_column += 1;
            }
        }
    }

    g_rendered = !g_rendered;
}

void rotateCW180(unsigned char *buffer_frame, unsigned char *rendered_frame) {
	int render_column = g_width - 1;
    int render_row = g_height - 1;
    temp = g_width * 3;
    for (int row = 0; row < g_width; row+=T) {
        for (int column = 0; column < g_height; column+=T) {
            for (int row_t = row; row_t < min(row + T, g_width); row_t++) {
                temp2 = render_row * temp;
                temp4 = row_t * temp;
                for (int column_t = column; column_t < min(column + T, g_height); column_t++) {
                    temp1 = temp2 + render_column * 3;
                    int position_frame_buffer = temp4 + column_t * 3;
                    rendered_frame[temp1] = buffer_frame[position_frame_buffer];
                    rendered_frame[temp1 + 1] = buffer_frame[position_frame_buffer + 1];
                    rendered_frame[temp1 + 2] = buffer_frame[position_frame_buffer + 2];
                    render_column -= 1;
                }
                render_row -= 1;
                render_column = g_width - 1;
            }
        }
    }

    g_rendered = !g_rendered;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
void processRotateCW(unsigned char *buffer_frame, unsigned char *rendered_frame, int rotate_iteration) {
    if (rotate_iteration < 0){ // handle negative offsets
        processRotateCCW(buffer_frame, rendered_frame, -rotate_iteration);
        return;
    } else if (rotate_iteration % 4 == 0) {
        return;
	} else if (rotate_iteration % 4 == 1) {
        rotateCW90(buffer_frame, rendered_frame);
        return;
	} else if (rotate_iteration % 4 == 2) {
        rotateCW180(buffer_frame, rendered_frame);
        return;
	} else {
        rotateCCW90(buffer_frame, rendered_frame);		
        return;
	}
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer counter clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
void processRotateCCW(unsigned char *buffer_frame, unsigned char *rendered_frame, int rotate_iteration) {
    if (rotate_iteration < 0){ // handle negative offsets
        processRotateCW(buffer_frame, rendered_frame, -rotate_iteration);
        return;
    } else if (rotate_iteration % 4 == 0) {
		return;	
	} else if (rotate_iteration % 4 == 1) {
        rotateCCW90(buffer_frame, rendered_frame);
        return;
	} else if (rotate_iteration % 4 == 2) {
        rotateCW180(buffer_frame, rendered_frame);
        return;
	} else {
        rotateCW90(buffer_frame, rendered_frame);		
        return;
	}
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
void processMirrorX(unsigned char *buffer_frame, unsigned char *rendered_frame, int _unused) {
    temp = g_height * 3;
    // store shifted pixels to temporary buffer
    for (int row = 0; row < g_height; row+=T) {
        for (int column = 0; column < g_width; column+=T) {
            for (int row_t = row; row_t < min(row + T, g_height); row_t++) {
                temp1 = row_t * temp;
				temp2 = (g_height - row_t - 1) * temp;
                for (int column_t = column; column_t < min(column + T, g_width); column_t++) {
                    temp3 = column_t * 3;
				    int position_rendered_frame = temp1 + temp3;
                    int position_buffer_frame = temp2 + temp3;
                    
                    rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
                    rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
                    rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
                }
            }
        }
    }

    g_rendered = !g_rendered;
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
void processMirrorY(unsigned char *buffer_frame, unsigned char *rendered_frame, int _unused) {
    temp = g_height * 3;
    // store shifted pixels to temporary buffer
    for (int row = 0; row < g_height; row+=T) {
        for (int column = 0; column < g_width; column+=T) {
            for (int row_t = row; row_t < min(row + T, g_height); row_t++) {
                temp1 = row_t * temp;
                for (int column_t = column; column_t < min(column + T, g_width); column_t++) {
				    int position_rendered_frame = temp1 + column_t * 3;
				    int position_buffer_frame = temp1 + (g_width - column_t - 1) * 3;
				    rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
				    rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
				    rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
				}
			}
        }
    }

    g_rendered = !g_rendered;
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "C99";

    // Please fill in your information
    char student1_first_name[] = "Qianhao";
    char student1_last_name[] = "Zhang";
    char student1_student_number[] = "1004654377";

    // Please fill in your partner's information
    // If yon't have partner, do not modify this
    char student2_first_name[] = "Jingfeng";
    char student2_last_name[] = "Chen";
    char student2_student_number[] = "1000411262";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent1_first_name: %s\n", student1_first_name);
    printf("\tstudent1_last_name: %s\n", student1_last_name);
    printf("\tstudent1_student_number: %s\n", student1_student_number);
    printf("\tstudent2_first_name: %s\n", student2_first_name);
    printf("\tstudent2_last_name: %s\n", student2_last_name);
    printf("\tstudent2_student_number: %s\n", student2_student_number);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          You can modify anything else in this file
 ***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported  24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
	// assign global variables
	g_width = width;
	g_height = height;

    // allocate memory for temporary image buffer
    unsigned char *frame_rendered = allocateFrame(width, height);
    
    int processed_frames = 0;
    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
//        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
//               sensor_values[sensorValueIdx].value);
        if (!strcmp(sensor_values[sensorValueIdx].key, "W")) {
            if (g_rendered) {
                processMoveUp(frame_rendered, frame_buffer, sensor_values[sensorValueIdx].value);
            } else {
                processMoveUp(frame_buffer, frame_rendered, sensor_values[sensorValueIdx].value);
            }
//          printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) {
            if (g_rendered) {
                processMoveLeft(frame_rendered, frame_buffer, sensor_values[sensorValueIdx].value);
            } else {
                processMoveLeft(frame_buffer, frame_rendered, sensor_values[sensorValueIdx].value);
            }
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) {
            if (g_rendered) {
                processMoveDown(frame_rendered, frame_buffer, sensor_values[sensorValueIdx].value);
            } else {
                processMoveDown(frame_buffer, frame_rendered, sensor_values[sensorValueIdx].value);
            }
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {
            if (g_rendered) {
                processMoveRight(frame_rendered, frame_buffer, sensor_values[sensorValueIdx].value);
            } else {
                processMoveRight(frame_buffer, frame_rendered, sensor_values[sensorValueIdx].value);
            }
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CW")) {
            if (g_rendered) {
                processRotateCW(frame_rendered, frame_buffer, sensor_values[sensorValueIdx].value);
            } else {
                processRotateCW(frame_buffer, frame_rendered, sensor_values[sensorValueIdx].value);
            }
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) {
            if (g_rendered) {
                processRotateCCW(frame_rendered, frame_buffer, sensor_values[sensorValueIdx].value);
            } else {
                processRotateCCW(frame_buffer, frame_rendered, sensor_values[sensorValueIdx].value);
            }
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
            if (g_rendered) {
                processMirrorX(frame_rendered, frame_buffer, sensor_values[sensorValueIdx].value);
            } else {
                processMirrorX(frame_buffer, frame_rendered, sensor_values[sensorValueIdx].value);
            }
//            printBMP(width, height, frame_buffer);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
            if (g_rendered) {
                processMirrorY(frame_rendered, frame_buffer, sensor_values[sensorValueIdx].value);
            } else {
                processMirrorY(frame_buffer, frame_rendered, sensor_values[sensorValueIdx].value);
            }
//            printBMP(width, height, frame_buffer);
        }
        processed_frames += 1;
		if (processed_frames % 25 == 0) {
			if (g_rendered) {
                verifyFrame(frame_rendered, width, height, grading_mode);
		    } else {
		        verifyFrame(frame_buffer, width, height, grading_mode);
		    }
		}
    }
    if (g_rendered) {
        frame_buffer = copyFrame(frame_rendered, frame_buffer, g_width, g_height);
    }

    // free temporary image buffer
    deallocateFrame(frame_rendered);
    return;
}