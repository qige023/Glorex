#pragma once
// Std. Includes
#include <vector>

// GL Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <esutil.h>

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class ESCamera {
public:
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum Movement {
        FORWARD, BACKWARD, LEFT, RIGHT
    };

    // Default camera values
    static const GLfloat YAW = -90.0f;
    static const GLfloat PITCH = 0.0f;
    static const GLfloat SPEED = 3.0f;
    static const GLfloat SENSITIVTY = 0.75f; //0.25f
    static const GLfloat ZOOM = 45.0f;

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;

    // Camera options
    GLfloat MovementSpeed;
    GLfloat RotateSensitivity;
    GLfloat Zoom;

    // Constructor with vectors
    ESCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up =
            glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch =
            PITCH) :
            Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), RotateSensitivity(
                    SENSITIVTY), Zoom(ZOOM) {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }
    // Constructor with scalar values
    ESCamera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY,
            GLfloat upZ, GLfloat yaw, GLfloat pitch) :
            Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), RotateSensitivity(
                    SENSITIVTY), Zoom(ZOOM) {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

    void moveByDirection(Movement direction, GLfloat deltaTime) {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void rotate(GLfloat xoffset, GLfloat yoffset,
            GLboolean constrainPitch = true) {
        xoffset *= this->RotateSensitivity;
        yoffset *= this->RotateSensitivity;

        this->Yaw += xoffset;
        this->Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }

    void zoomInOut(GLfloat yoffset) {
        if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
            this->Zoom -= yoffset;
        if (this->Zoom <= 1.0f)
            this->Zoom = 1.0f;
        if (this->Zoom >= 45.0f)
            this->Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors() {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};
