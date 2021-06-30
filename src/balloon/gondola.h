#ifndef GONDOLA_H
#define GONDOLA_H

#include <gamelib/aero_object.h>
#include <gamelib/vector2.h>

#include <vector>

#include <allegro5/bitmap.h>

/**
 * @brief Provides information for the balloon gondola
 */
class Gondola : public AeroObject
{
public:
    /**
     * @brief Constructs the core balloon gondola
     */
    Gondola();

    /**
     * @brief Provides the corresponding Gondola point
     * @return the top-left Gondola point in global coordinates
     */
    Vector2 get_top_left() const;

    /**
     * @brief Provides the corresponding Gondola point
     * @return the top-right Gondola point in global coordinates
     */
    Vector2 get_top_right() const;

    /**
     * @brief Provides the corresponding Gondola point
     * @return the bottom-left Gondola point in global coordinates
     */
    Vector2 get_bottom_left() const;

    /**
     * @brief Provides the corresponding Gondola point
     * @return the bottom-right Gondola point in global coordinates
     */
    Vector2 get_bottom_right() const;

    /**
     * @brief Draws the balloon gondola onto the current display
     * @param state the draw state for the current object
     */
    void draw(const DrawState* state) override;

    /**
     * @brief Adds the correct forces to the Gondola
     * @param state the step state to utilize
     */
    void pre_step(const StepState* state) override;

    /**
     * @brief Provides an update on the default step parameters to run right before performing the physics step
     * @param state the step state to utilize
    */
    void step(const StepState* state) override;

    /* Destructor */
    ~Gondola();

protected:
    std::vector<Vector2> get_points() const;

protected:
    double width;
    double height;

    ALLEGRO_BITMAP* bitmap;
};

#endif // GONDOLA_H
