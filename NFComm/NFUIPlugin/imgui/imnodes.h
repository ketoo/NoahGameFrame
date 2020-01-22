#pragma once

#include <stddef.h>

struct ImVec2;

namespace imnodes
{
enum ColorStyle
{
    ColorStyle_NodeBackground = 0,
    ColorStyle_NodeBackgroundHovered,
    ColorStyle_NodeBackgroundSelected,
    ColorStyle_NodeOutline,
    ColorStyle_TitleBar,
    ColorStyle_TitleBarHovered,
    ColorStyle_TitleBarSelected,
    ColorStyle_Link,
    ColorStyle_LinkHovered,
    ColorStyle_LinkSelected,
    ColorStyle_Pin,
    ColorStyle_PinHovered,
    ColorStyle_PinOutline,
    ColorStyle_BoxSelector,
    ColorStyle_BoxSelectorOutline,
    ColorStyle_GridBackground,
    ColorStyle_GridLine,
    ColorStyle_Count
};

enum StyleVar
{
    StyleVar_GridSpacing = 0,
    StyleVar_NodeCornerRounding,
    StyleVar_NodePaddingHorizontal,
    StyleVar_NodePaddingVertical
};

enum StyleFlags
{
    StyleFlags_None = 0,
    StyleFlags_NodeOutline = 1 << 0,
    StyleFlags_PinOutline = 1 << 1,
    StyleFlags_GridLines = 1 << 2
};

struct Style
{
    float grid_spacing = 32.f;

    float node_corner_rounding = 4.0f;
    float node_padding_horizontal = 8.0f;
    float node_padding_vertical = 8.0f;

    float link_thickness = 3.0f;
    float link_line_segments_per_length = 0.1f;
    float link_hover_distance = 7.0f;

    float pin_radius = 4.0f;
    float pin_hover_radius = 10.0f;

    StyleFlags flags =
        StyleFlags(StyleFlags_NodeOutline | StyleFlags_GridLines);
    // Set these mid-frame using Push/PopColorStyle. You can index this color
    // array with with a ColorStyle enum value.
    unsigned int colors[ColorStyle_Count];
};

// An editor context corresponds to a set of nodes in a single workspace
// (created with a single Begin/EndNodeEditor pair)
//
// By default, the library creates an editor context behind the scenes, so
// using any of the imnodes functions doesn't require you to explicitly create a
// context.
struct EditorContext;

EditorContext* EditorContextCreate();
void EditorContextFree(EditorContext*);
void EditorContextSet(EditorContext*);
void EditorContextResetPanning(const ImVec2& pos);
void EditorContextMoveToNode(const int node_id);

// Initialize the node editor system.
void Initialize();
void Shutdown();

// Returns the global style struct. See the struct declaration for default
// values.
Style& GetStyle();
// Style presets matching the dear imgui styles of the same name.
void StyleColorsDark(); // on by default
void StyleColorsClassic();
void StyleColorsLight();

// The top-level function call. Call this before calling BeginNode/EndNode.
// Calling this function will result the node editor grid workspace being
// rendered.
void BeginNodeEditor();
void EndNodeEditor();

// Use PushColorStyle and PopColorStyle to modify Style::colors mid-frame.
void PushColorStyle(ColorStyle item, unsigned int color);
void PopColorStyle();
void PushStyleVar(StyleVar style_item, float value);
void PopStyleVar();

void BeginNode(int id);
void EndNode();

// Attributes are ImGui UI elements embedded within the node. Attributes have
// circular pins rendered next to them. Links are created between pins.
//
// Input and output attributes are otherwise the same, except that pins are
// rendered on the left of the node for input attributes, and on the right side
// for output attributes.
//
// The attribute ids must be unique.
void BeginInputAttribute(int id);
void BeginOutputAttribute(int id);
void EndAttribute();

// Render a link between attributes.
// The attributes ids used here must match the ids used in
// Begin(Input|Output)Attribute function calls. The order of start_attr and
// end_attr doesn't make a difference for rendering the link.
void Link(int id, int start_attr, int end_attr);

// Set's the node's position corresponding to the node id, either using screen
// space coordinates, or node editor grid coordinates. You can even set the
// position before the node has been created with BeginNode().

void SetNodeScreenSpacePos(int node_id, const ImVec2& screen_space_pos);
void SetNodeGridSpacePos(int node_id, const ImVec2& grid_pos);

// Set the node name corresponding to the node id. The node name is displayed in
// the node's title bar.
void SetNodeName(int node_id, const char* name);
// Enable or disable the ability to click and drag a specific node.
void SetNodeDraggable(int node_id, const bool dragable);

// The following functions return true if a UI element is being hovered over by
// the mouse cursor. Assigns the id of the UI element being hovered over to the
// function argument. Use these functions after EndNodeEditor() has been called.
bool IsNodeHovered(int* node_id);
bool IsLinkHovered(int* link_id);
bool IsPinHovered(int* attribute_id);

// Use The following two functions to query the number of selected nodes or
// links in the current editor. Use after calling EndNodeEditor().
int NumSelectedNodes();
int NumSelectedLinks();
// Get the selected node/link ids. The pointer argument should point to an
// integer array with at least as many elements as the respective
// NumSelectedNodes/NumSelectedLinks function call returned.
void GetSelectedNodes(int* node_ids);
void GetSelectedLinks(int* link_ids);

// Was the previous attribute active? This will continuously return true while
// the left mouse button is being pressed over the UI content of the attribute.
bool IsAttributeActive();
// Was any attribute active? If so, sets the active attribute id to the output
// function argument.
bool IsAnyAttributeActive(int* attribute_id = 0);

// The following functions should be used after calling EndNodeEditor().
//
// Is the user dragging a new link?
bool IsLinkStarted(int* started_at_attr);
// Did the user drop the new link before connecting it to a second attribute?
bool IsLinkDropped();
// Did the user create a new link?
bool IsLinkCreated(int* started_at_attr, int* ended_at_attr);

// Use the following functions to write the editor context's state to a string,
// or directly to a file. The editor context is serialized in the INI file
// format.

const char* SaveCurrentEditorStateToIniString(size_t* data_size = NULL);
const char* SaveEditorStateToIniString(
    const EditorContext* editor,
    size_t* data_size = NULL);

void LoadCurrentEditorStateFromIniString(const char* data, size_t data_size);
void LoadEditorStateFromIniString(
    EditorContext* editor,
    const char* data,
    size_t data_size);

void SaveCurrentEditorStateToIniFile(const char* file_name);
void SaveEditorStateToIniFile(
    const EditorContext* editor,
    const char* file_name);

void LoadCurrentEditorStateFromIniFile(const char* file_name);
void LoadEditorStateFromIniFile(EditorContext* editor, const char* file_name);
} // namespace imnodes
