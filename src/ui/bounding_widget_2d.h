#ifndef BOUNDING_POLYGON_WIDGET_H
#define BOUNDING_POLYGON_WIDGET_H

#include <Eigen/Core>
#include "glad/glad.h"
#include "state.h"
#include "volume_exporter.h"


namespace igl { namespace opengl { namespace glfw { class Viewer; }}}

class Bounding_Polygon_Widget {
public:
    Bounding_Polygon_Widget(State& state);

    void initialize(igl::opengl::glfw::Viewer* viewer);

    bool mouse_move(int mouse_x, int mouse_y);
    bool mouse_down(int button, int modifier);
    bool mouse_up(int button, int modifier);
    bool mouse_scroll(float delta_y);

    bool post_draw(BoundingCage::KeyFrameIterator it, int current_vertex_id);


    glm::vec2 position = { 0.f, 0.f }; // window coordinates in pixels lower left of window
    glm::vec2 size = { 500.f, 500.f }; // window coordinates in pixels

    const float SelectionRadius = 0.15f;
    const float MaxZoomLevel = 200.f;
    float polygon_point_size = 8.f;
    float polygon_line_width = 3.f;
    float selected_point_size = 10.f;
    float split_point_size = 7.f;
    float center_point_size = 12.f;
    float selected_center_point_size = 14.f;

    glm::vec4 polygon_line_color = glm::vec4(0.85f, 0.85f, 0.f, 1.f);
    glm::vec4 selected_point_color = glm::vec4(0.8f, 0.2f, 0.2f, 1.f);
    glm::vec4 available_point_color = glm::vec4(0.2f, 0.8f, 0.2f, 1.f);
    glm::vec4 center_point_color = glm::vec4(0.2f, 0.2f, 0.8f, 1.f);
    glm::vec4 selected_center_point_color = glm::vec4(0.2f, 0.5f, 0.8f, 1.f);

private:
    bool point_in_widget(const glm::ivec2& p) const;
    glm::vec2 convert_position_mainwindow_to_keyframe(const glm::vec2& p) const;
    glm::vec2 convert_position_keyframe_to_ndc(const glm::vec2& p) const;

    void update_selection();

    State& state;
    igl::opengl::glfw::Viewer* viewer;

    // State to track pan and zoom
    struct {
        glm::vec2 offset; // in kf
        float zoom = 10.f;
    } view;

    // State tracking cursor position and button state
    struct {
        glm::ivec2 current_position; // main window coordinates
        glm::vec2 down_position;     // main window coordinates
        bool is_left_button_down = false;
        bool is_right_button_down = false;
        float scroll = 0.f;
        bool is_ctrl_down = false;
    } mouse_state;

    // State tracking what elements are being edited
    static constexpr const int NoElement = -1;
    static constexpr const int CenterElement = -2;
    struct {
        bool matched_center = false;
        bool matched_vertex = false;
        int closest_vertex_index = NoElement;
        int current_edit_element = NoElement;
        BoundingCage::KeyFrameIterator current_active_keyframe;
    } selection;

    // OpenGL handles
    GLuint empty_vao = 0;
    struct {
        GLuint program = 0;

        GLint window_size_location = -1;
        GLint ll_location = -1;
        GLint lr_location = -1;
        GLint ul_location = -1;
        GLint ur_location = -1;

        GLint texture_location = -1;
        GLint tf_location = -1;
    } plane;

    struct {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint program = 0;

        GLint color_location = -1;
    } polygon;

    struct {
        GLuint fbo = 0;
        GLuint texture = 0;
        glm::ivec2 texture_size = { 1024, 1024 };
    } offscreen;

    struct {
        GLuint program = 0;
        GLuint vao = 0;
        GLuint vbo = 0;

        GLint texture_location = -1;
    } blit;
};

#endif // BOUNDING_POLYGON_WIDGET_H
