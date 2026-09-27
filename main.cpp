#include <windows.h>
#include <GL/glut.h>
#include <math.h>

// Global variables for animation and state
GLfloat position = 0.0f; // For sun/moon animation
GLfloat position3 = 0.0f; // For clouds animation
GLfloat boatShipPos = 0.0f; // For boats/ship animation
GLfloat waveHeight = 0.0f; // For the sea waves
GLfloat speed = 0.05f;

#define PI 3.14159265358979323846
GLfloat twicePi = 2.0f * PI;
int triangleAmount = 100;

bool night = false;
bool waveUp = true;

// Color of the sun during the day
GLfloat sun_r = 1.0f, sun_g = 0.9f, sun_b = 0.0f;
// Color of the moon at night
GLfloat moon_r = 0.95f, moon_g = 0.95f, moon_b = 0.9f;

// --- Function to draw a cloud ---
void drawCloud(GLfloat x, GLfloat y) {
    if (night) {
        glColor3f(0.4f, 0.4f, 0.45f); // Darker cloud for night
    } else {
        glColor3f(0.95f, 0.95f, 0.95f); // Lighter cloud for day
    }

    // Draw cloud using overlapping circles
    GLfloat radius = 0.05f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for(int i = 0; i <= triangleAmount; i++) {
        glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
                   y + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + 0.04f, y - 0.01f);
    for(int i = 0; i <= triangleAmount; i++) {
        glVertex2f((x + 0.04f) + (radius * cos(i * twicePi / triangleAmount)),
                   (y - 0.01f) + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x - 0.04f, y + 0.01f);
    for(int i = 0; i <= triangleAmount; i++) {
        glVertex2f((x - 0.04f) + (radius * cos(i * twicePi / triangleAmount)),
                   (y + 0.01f) + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}

// --- Function to draw a boat ---
void drawBoat(GLfloat x, GLfloat y) {
    if (night) {
        glColor3f(0.4f, 0.2f, 0.0f); // Darker boat for night
    } else {
        glColor3f(0.6f, 0.4f, 0.2f); // Lighter boat for day
    }
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.05f, y);
    glVertex2f(x + 0.05f, y);
    glVertex2f(x + 0.03f, y + 0.02f);
    glVertex2f(x - 0.03f, y + 0.02f);
    glEnd();

    // Mast
    if (night) {
        glColor3f(0.2f, 0.2f, 0.2f);
    } else {
        glColor3f(0.5f, 0.5f, 0.5f);
    }
    glBegin(GL_LINES);
    glVertex2f(x, y + 0.02f);
    glVertex2f(x, y + 0.08f);
    glEnd();

    // Sail
    if (night) {
        glColor3f(0.8f, 0.8f, 0.8f);
    } else {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 0.08f);
    glVertex2f(x, y + 0.02f);
    glVertex2f(x + 0.04f, y + 0.04f);
    glEnd();
}

// --- Function to draw a ship ---
void drawShip(GLfloat x, GLfloat y) {
    if (night) {
        glColor3f(0.2f, 0.2f, 0.2f); // Darker ship for night
    } else {
        glColor3f(0.4f, 0.4f, 0.4f); // Lighter ship for day
    }
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.1f, y);
    glVertex2f(x + 0.1f, y);
    glVertex2f(x + 0.08f, y + 0.03f);
    glVertex2f(x - 0.08f, y + 0.03f);
    glEnd();

    // Masts
    if (night) {
        glColor3f(0.3f, 0.3f, 0.3f);
    } else {
        glColor3f(0.6f, 0.6f, 0.6f);
    }
    glBegin(GL_LINES);
    glVertex2f(x - 0.05f, y + 0.03f);
    glVertex2f(x - 0.05f, y + 0.1f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x + 0.05f, y + 0.03f);
    glVertex2f(x + 0.05f, y + 0.1f);
    glEnd();
}

// --- Function to draw a realistic coconut tree ---
void drawCoconutTree(float x, float y) {
    // Trunk - more realistic with curved shape
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.015f, y);
    glVertex2f(x - 0.02f, y + 0.1f);
    glVertex2f(x - 0.015f, y + 0.2f);
    glVertex2f(x + 0.015f, y + 0.2f);
    glVertex2f(x + 0.02f, y + 0.1f);
    glVertex2f(x + 0.015f, y);
    glEnd();

    // Leaves - more realistic with curved fronds
    if (night) {
        glColor3f(0.0f, 0.3f, 0.0f);
    } else {
        glColor3f(0.0f, 0.6f, 0.2f);
    }

    // Main fronds
    for (int i = 0; i < 8; i++) {
        float angle = i * 45.0f * (PI / 180.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x, y + 0.25f);
        glVertex2f(x + 0.12f * cos(angle - 0.2), y + 0.25f + 0.12f * sin(angle - 0.2));
        glVertex2f(x + 0.12f * cos(angle + 0.2), y + 0.25f + 0.12f * sin(angle + 0.2));
        glEnd();
    }

    // Additional smaller fronds for density
    for (int i = 0; i < 8; i++) {
        float angle = (i * 45.0f + 22.5f) * (PI / 180.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x, y + 0.25f);
        glVertex2f(x + 0.08f * cos(angle - 0.15), y + 0.25f + 0.08f * sin(angle - 0.15));
        glVertex2f(x + 0.08f * cos(angle + 0.15), y + 0.25f + 0.08f * sin(angle + 0.15));
        glEnd();
    }

    // Coconuts - more realistic placement
    glColor3f(0.4f, 0.2f, 0.1f);
    float radius = 0.015f;
    for (int i = 0; i < 4; i++) {
        float offsetX = (i % 2) * 0.03f - 0.015f;
        float offsetY = (i < 2) ? 0.22f : 0.18f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x + offsetX, y + offsetY);
        for (int j = 0; j <= triangleAmount; j++) {
            glVertex2f(
                x + offsetX + radius * cos(j * twicePi / triangleAmount),
                y + offsetY + radius * sin(j * twicePi / triangleAmount)
            );
        }
        glEnd();
    }
}

// --- Function to draw a person ---
void drawPerson(GLfloat x, GLfloat y, bool handup) {
    // Head
    GLfloat radiusp = 0.01f;
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y + 0.1f);
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + radiusp * cos(i * twicePi / triangleAmount),
            y + 0.1f + radiusp * sin(i * twicePi / triangleAmount)
        );
    }
    glEnd();

    // Body (shirt)
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.8f, 0.0f);
    glVertex2f(x - 0.007f, y + 0.09f); glVertex2f(x + 0.007f, y + 0.09f);
    glVertex2f(x + 0.007f, y + 0.03f); glVertex2f(x - 0.007f, y + 0.03f);
    glEnd();

    // Hands
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f);
    if (!handup) {
        // Left hand down
        glVertex2f(x - 0.01f, y + 0.08f); glVertex2f(x - 0.007f, y + 0.08f);
        glVertex2f(x - 0.007f, y + 0.03f); glVertex2f(x - 0.01f, y + 0.03f);
    } else {
        // Left hand waving
        glVertex2f(x - 0.01f, y + 0.08f); glVertex2f(x - 0.007f, y + 0.08f);
        glVertex2f(x - 0.007f, y + 0.12f); glVertex2f(x - 0.01f, y + 0.12f);
    }
    // Right hand down
    glVertex2f(x + 0.01f, y + 0.08f); glVertex2f(x + 0.007f, y + 0.08f);
    glVertex2f(x + 0.007f, y + 0.03f); glVertex2f(x + 0.01f, y + 0.03f);
    glEnd();

    // Legs
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(x - 0.005f, y + 0.03f); glVertex2f(x, y + 0.03f);
    glVertex2f(x, y); glVertex2f(x - 0.005f, y);
    glVertex2f(x + 0.005f, y + 0.03f); glVertex2f(x, y + 0.03f);
    glVertex2f(x, y); glVertex2f(x + 0.005f, y);
    glEnd();
}

// --- Function to draw a realistic beach chair ---
void drawChair(GLfloat x, GLfloat y) {
    // Chair frame
    if (night) {
        glColor3f(0.3f, 0.3f, 0.3f);
    } else {
        glColor3f(0.7f, 0.5f, 0.3f); // Wood color
    }

    // Seat frame
    glBegin(GL_QUADS);
    glVertex2f(x - 0.06f, y + 0.05f);
    glVertex2f(x + 0.06f, y + 0.05f);
    glVertex2f(x + 0.06f, y + 0.03f);
    glVertex2f(x - 0.06f, y + 0.03f);
    glEnd();

    // Backrest frame
    glBegin(GL_QUADS);
    glVertex2f(x - 0.06f, y + 0.05f);
    glVertex2f(x - 0.04f, y + 0.05f);
    glVertex2f(x - 0.04f, y + 0.12f);
    glVertex2f(x - 0.06f, y + 0.12f);
    glEnd();

    // Chair fabric
    if (night) {
        glColor3f(0.2f, 0.2f, 0.4f);
    } else {
        glColor3f(0.8f, 0.2f, 0.2f); // Red fabric
    }

    // Seat
    glBegin(GL_QUADS);
    glVertex2f(x - 0.055f, y + 0.045f);
    glVertex2f(x + 0.055f, y + 0.045f);
    glVertex2f(x + 0.055f, y + 0.035f);
    glVertex2f(x - 0.055f, y + 0.035f);
    glEnd();

    // Backrest
    glBegin(GL_QUADS);
    glVertex2f(x - 0.055f, y + 0.05f);
    glVertex2f(x - 0.045f, y + 0.05f);
    glVertex2f(x - 0.045f, y + 0.115f);
    glVertex2f(x - 0.055f, y + 0.115f);
    glEnd();

    // Legs
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.05f, y + 0.03f); glVertex2f(x - 0.05f, y);
    glVertex2f(x + 0.05f, y + 0.03f); glVertex2f(x + 0.05f, y);
    glVertex2f(x - 0.05f, y); glVertex2f(x - 0.03f, y - 0.02f);
    glVertex2f(x + 0.05f, y); glVertex2f(x + 0.03f, y - 0.02f);
    glEnd();
}

// --- Function to draw a lamp post ---
void drawLampPost(GLfloat x, GLfloat y) {
    // Pole
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.005f, y);
    glVertex2f(x + 0.005f, y);
    glVertex2f(x + 0.005f, y + 0.28f);
    glVertex2f(x - 0.005f, y + 0.28f);
    glEnd();

    // Light source
    if (night) {
        glColor3f(0.8f, 0.8f, 0.0f); // Light on
        // Light glow effect
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y + 0.29f);
        for(int i = 0; i <= triangleAmount; i++) {
            glVertex2f(x + (0.08f * cos(i * twicePi / triangleAmount)),
                       y + 0.29f + (0.08f * sin(i * twicePi / triangleAmount)));
        }
        glEnd();
    } else {
        glColor3f(0.2f, 0.2f, 0.2f); // Light off
    }

    // Lamp housing
    glBegin(GL_QUADS);
    glVertex2f(x - 0.02f, y + 0.3f);
    glVertex2f(x + 0.02f, y + 0.3f);
    glVertex2f(x + 0.02f, y + 0.28f);
    glVertex2f(x - 0.02f, y + 0.28f);
    glEnd();
}

// --- Function to draw a realistic beach umbrella ---
void drawUmbrella(GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b) {
    // Umbrella canopy with more realistic shape
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y + 0.15f); // Top center
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + radius * cos(i * twicePi / triangleAmount),
            y + 0.08f + radius * sin(i * twicePi / triangleAmount) * 0.7f
        );
    }
    glEnd();

    // Umbrella ribs
    glColor3f(0.3f, 0.3f, 0.3f);
    for (int i = 0; i < 8; i++) {
        float angle = i * 45.0f * (PI / 180.0f);
        glBegin(GL_LINES);
        glVertex2f(x, y + 0.15f);
        glVertex2f(
            x + radius * cos(angle),
            y + 0.08f + radius * sin(angle) * 0.7f
        );
        glEnd();
    }

    // Pole
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.008f, y);
    glVertex2f(x + 0.008f, y);
    glVertex2f(x + 0.008f, y + 0.15f);
    glVertex2f(x - 0.008f, y + 0.15f);
    glEnd();

    // Base
    glBegin(GL_QUADS);
    glVertex2f(x - 0.03f, y);
    glVertex2f(x + 0.03f, y);
    glVertex2f(x + 0.03f, y - 0.02f);
    glVertex2f(x - 0.03f, y - 0.02f);
    glEnd();
}

// --- Function to draw a tea stall ---
void drawTeaStall(GLfloat x, GLfloat y) {
    // Stall base
    if (night) {
        glColor3f(0.4f, 0.3f, 0.2f);
    } else {
        glColor3f(0.6f, 0.5f, 0.4f); // Wooden stall
    }
    glBegin(GL_QUADS);
    glVertex2f(x - 0.15f, y);
    glVertex2f(x + 0.15f, y);
    glVertex2f(x + 0.15f, y + 0.15f);
    glVertex2f(x - 0.15f, y + 0.15f);
    glEnd();

    // Stall roof
    if (night) {
        glColor3f(0.3f, 0.2f, 0.1f);
    } else {
        glColor3f(0.8f, 0.4f, 0.2f); // Brown roof
    }
    glBegin(GL_QUADS);
    glVertex2f(x - 0.18f, y + 0.15f);
    glVertex2f(x + 0.18f, y + 0.15f);
    glVertex2f(x + 0.18f, y + 0.2f);
    glVertex2f(x - 0.18f, y + 0.2f);
    glEnd();

    // Roof top
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.18f, y + 0.2f);
    glVertex2f(x + 0.18f, y + 0.2f);
    glVertex2f(x, y + 0.25f);
    glEnd();

    // Counter
    if (night) {
        glColor3f(0.2f, 0.2f, 0.2f);
    } else {
        glColor3f(0.3f, 0.3f, 0.3f); // Gray counter
    }
    glBegin(GL_QUADS);
    glVertex2f(x - 0.1f, y + 0.05f);
    glVertex2f(x + 0.1f, y + 0.05f);
    glVertex2f(x + 0.1f, y + 0.1f);
    glVertex2f(x - 0.1f, y + 0.1f);
    glEnd();

    // Tea pot
    glColor3f(0.8f, 0.8f, 0.8f); // Silver pot
    glBegin(GL_QUADS);
    glVertex2f(x - 0.03f, y + 0.1f);
    glVertex2f(x + 0.03f, y + 0.1f);
    glVertex2f(x + 0.03f, y + 0.13f);
    glVertex2f(x - 0.03f, y + 0.13f);
    glEnd();

    // Tea pot lid
    glBegin(GL_QUADS);
    glVertex2f(x - 0.02f, y + 0.13f);
    glVertex2f(x + 0.02f, y + 0.13f);
    glVertex2f(x + 0.02f, y + 0.14f);
    glVertex2f(x - 0.02f, y + 0.14f);
    glEnd();

    // Tea cups
    glColor3f(0.9f, 0.9f, 0.9f); // White cups
    for (int i = 0; i < 3; i++) {
        float offset = i * 0.05f - 0.05f;
        glBegin(GL_QUADS);
        glVertex2f(x + offset - 0.01f, y + 0.05f);
        glVertex2f(x + offset + 0.01f, y + 0.05f);
        glVertex2f(x + offset + 0.01f, y + 0.08f);
        glVertex2f(x + offset - 0.01f, y + 0.08f);
        glEnd();
    }

    // Sign board
    if (night) {
        glColor3f(0.7f, 0.5f, 0.3f);
    } else {
        glColor3f(0.9f, 0.7f, 0.5f);
    }
    glBegin(GL_QUADS);
    glVertex2f(x - 0.08f, y + 0.2f);
    glVertex2f(x + 0.08f, y + 0.2f);
    glVertex2f(x + 0.08f, y + 0.22f);
    glVertex2f(x - 0.08f, y + 0.22f);
    glEnd();

    // Sign text
    glColor3f(1.0f, 0.0f, 0.0f); // Red text
    glRasterPos2f(x - 0.05f, y + 0.205f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, 'T');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, 'E');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, 'A');
}

// --- Display function to render the scene ---
void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // ----------------- Sky -------------------
    glBegin(GL_QUADS);
    if (night) {
        glColor3f(0.18f, 0.36f, 0.56f); // Dark blue sky
    } else {
        glColor3f(0.4f, 0.69f, 1.0f); // Light blue sky
    }
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(1.0f, 0.5f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // ----------------- Sun or Moon -------------------
    // Stable sun/moon animation - moves from right to left and resets
    GLfloat xs1 = position;
    GLfloat ys1 = 0.85f;
    GLfloat radiuss1 = 0.07f;

    if (!night) {
        // Sun
        glColor3f(sun_r, sun_g, sun_b);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(xs1, ys1);
        for(int i = 0; i <= triangleAmount; i++) {
            glVertex2f( xs1 + (radiuss1 * cos(i * twicePi / triangleAmount)),
                        ys1 + (radiuss1 * sin(i * twicePi / triangleAmount)) );
        }
        glEnd();

        // Sun rays
        glColor3f(1.0f, 1.0f, 0.5f);
        for(int i = 0; i < 12; i++) {
            float angle = i * 30.0f * (PI / 180.0f);
            glBegin(GL_LINES);
            glVertex2f(xs1, ys1);
            glVertex2f(xs1 + 0.12f * cos(angle), ys1 + 0.12f * sin(angle));
            glEnd();
        }
    } else {
        // Moon
        glColor3f(moon_r, moon_g, moon_b);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(xs1, ys1);
        for(int i = 0; i <= triangleAmount; i++) {
            glVertex2f( xs1 + (radiuss1 * cos(i * twicePi / triangleAmount)),
                        ys1 + (radiuss1 * sin(i * twicePi / triangleAmount)) );
        }
        glEnd();

        // Stars
        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        glVertex2f(-0.8f, 0.9f);
        glVertex2f(0.5f, 0.75f);
        glVertex2f(0.9f, 0.8f);
        glVertex2f(-0.4f, 0.95f);
        glVertex2f(0.1f, 0.88f);
        glVertex2f(-0.7f, 0.65f);
        glVertex2f(-0.2f, 0.85f);
        glVertex2f(0.3f, 0.95f);
        glVertex2f(-0.9f, 0.75f);
        glVertex2f(0.7f, 0.85f);
        glEnd();
    }

    // ----------------- Clouds -------------------
    glPushMatrix();
    glTranslatef(position3, 0.0f, 0.0f);
    drawCloud(-0.8f, 0.9f);
    drawCloud(-0.3f, 0.8f);
    drawCloud(0.2f, 0.9f);
    drawCloud(0.7f, 0.85f);
    drawCloud(-1.0f, 0.7f);
    drawCloud(-0.5f, 0.65f);
    drawCloud(0.0f, 0.75f);
    drawCloud(0.5f, 0.6f);
    glPopMatrix();

    // ----------------- Hills -----------------
    glBegin(GL_POLYGON);
    if(night){
        glColor3f(0.1f, 0.15f, 0.1f);
    } else {
        glColor3f(0.2f, 0.4f, 0.2f);
    }
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(-0.5f, 0.8f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, 0.65f);
    glVertex2f(1.0f, 0.5f);
    glEnd();

    // ----------------- Sea -------------------
    glBegin(GL_QUADS);
    if(night) {
        glColor3f(0.0f, 0.15f, 0.3f);
    } else {
        glColor3f(0.0f, 0.4f, 0.8f);
    }
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(1.0f, 0.5f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(-1.0f, 0.0f);
    glEnd();

    // Waves
    glBegin(GL_QUADS);
    if(night) {
        glColor3f(0.0f, 0.2f, 0.4f);
    } else {
        glColor3f(0.0f, 0.5f, 0.9f);
    }
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(1.0f, 0.5f);
    glVertex2f(1.0f, 0.5f - waveHeight);
    glVertex2f(-1.0f, 0.5f - waveHeight);
    glEnd();

    // ----------------- Boats and Ship -------------------
    glPushMatrix();
    glTranslatef(boatShipPos, 0.0f, 0.0f);
    // Added more boats and ships
    drawBoat(-0.7f, 0.3f + sin(boatShipPos*10.0f)*0.02f);
    drawBoat(0.2f, 0.4f + sin(boatShipPos*12.0f)*0.02f);
    drawBoat(-0.9f, 0.35f + sin(boatShipPos*8.0f)*0.02f);
    drawBoat(0.6f, 0.32f + sin(boatShipPos*9.0f)*0.02f);
    drawShip(-0.2f, 0.2f + sin(boatShipPos*8.0f)*0.02f);
    drawShip(0.4f, 0.25f + sin(boatShipPos*7.0f)*0.02f);
    drawShip(-0.5f, 0.15f + sin(boatShipPos*6.0f)*0.02f);
    glPopMatrix();

    // ----------------- Sand -------------------
    glBegin(GL_QUADS);
    if(night) {
        glColor3f(0.3f, 0.25f, 0.2f);
    } else {
        glColor3f(0.8f, 0.7f, 0.5f);
    }
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // ----------------- Tea Stall -------------------
    drawTeaStall(-0.7f, -0.3f);

    // ----------------- Coconut Trees -------------------
    drawCoconutTree(-0.75f, -0.6f);
    drawCoconutTree(0.75f, -0.6f);
    drawCoconutTree(-0.5f, -0.5f);
    drawCoconutTree(0.5f, -0.5f);
    drawCoconutTree(-0.9f, -0.55f);
    drawCoconutTree(0.9f, -0.55f);

    // ----------------- Beach Chairs & Umbrellas -------------------
    drawChair(-0.3f, -0.2f);
    drawUmbrella(-0.3f, -0.2f, 0.08f, 1.0f, 0.0f, 0.0f); // Red umbrella
    drawChair(0.0f, -0.3f);
    drawUmbrella(0.0f, -0.3f, 0.08f, 0.0f, 0.8f, 0.0f); // Green umbrella
    drawChair(0.3f, -0.2f);
    drawUmbrella(0.3f, -0.2f, 0.06f, 0.0f, 0.0f, 1.0f); // Blue umbrella


    // ----------------- Lamp Posts -------------------
    // Added more lamp posts
    drawLampPost(-0.9f, 0.0f);
    drawLampPost(0.9f, 0.0f);
    drawLampPost(-0.6f, 0.0f);
    drawLampPost(0.6f, 0.0f);
    drawLampPost(-0.3f, 0.0f);
    drawLampPost(0.3f, 0.0f);
    drawLampPost(0.0f, 0.0f);

    // ----------------- Person -------------------
    drawPerson(0.65f, -0.2f, true); // Person beside the coconut tree
    drawPerson(-0.6f, -0.25f, false); // Person near tea stall

    glFlush();
}

// --- Animation update function ---
void update(int value)
{
    // Sun/moon animation - stable movement from right to left
    if (position < -1.5f) {
        position = 1.5f;
        night = !night; // Switch between day and night when sun/moon goes off screen
    }
    position -= speed / 2;

    // Clouds animation
    if (position3 < -2.0f) {
        position3 = 2.0f;
    }
    position3 -= speed / 3;

    // Boats and ship animation
    if (boatShipPos < -2.0f) {
        boatShipPos = 2.0f;
    }
    boatShipPos -= speed / 4;

    // Wave height animation
    if (waveUp) {
        waveHeight += 0.005f;
        if (waveHeight > 0.1f) {
            waveUp = false;
        }
    } else {
        waveHeight -= 0.005f;
        if (waveHeight < 0.0f) {
            waveUp = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(100, update, 0); // Call again in 100 milliseconds
}

// --- Keyboard input function for regular keys ---
void keyboard(unsigned char key, int x, int y) {
    if (key == 'n' || key == 'N') {
        night = !night;
        glutPostRedisplay();
    }
}

// --- Keyboard input function for special keys ---
void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            speed += 0.01; // Increase speed
            break;
        case GLUT_KEY_DOWN:
            if (speed > 0.01) {
                speed -= 0.01; // Decrease speed
            }
            break;
    }
    glutPostRedisplay();
}

// --- Main function to initialize and run the application ---
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(750, 750);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Animated Beach Scene with Tea Stall");
    glutDisplayFunc(display);
    glutSpecialFunc(SpecialInput);
    glutKeyboardFunc(keyboard);

    // Start the animation
    glutTimerFunc(100, update, 0);

    glutMainLoop();
    return 0;
}
