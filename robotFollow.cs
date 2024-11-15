using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Class defining the robot's follow behavior
public class RobotFollowBehavior : MonoBehaviour {

    // Constants to define movement boundaries in the Z (forward/backward) and X (side-to-side) directions
    const float MAX_Z_BOUND = .4f;
    const float MIN_Z_BOUND = .3f;
    const float X_BOUND = .03f;

    // Singleton instance of RobotFollowBehavior to allow global access to this instance
    private static RobotFollowBehavior instance;

    // Public accessor for the singleton instance
    public static RobotFollowBehavior Instance {
        get { return instance; }
    }

    // Current image target (the object the robot is following)
    private DefaultTrackableEventHandler currImageTarget;

    // Boolean to control robot movement
    private bool canMove = true;

    // Awake is called when the script instance is being loaded
    private void Awake() {
        // Initialize singleton instance
        instance = this;
    }

    // Update is called once per frame
    private void Update() {
        
        // Check if there is a target being tracked
        if (currImageTarget != null) {

            // Check if the target is currently tracking and robot movement is allowed
            if (currImageTarget.isTracking && canMove) {

                // Move robot left if target's position is left of the X_BOUND
                if (currImageTarget.transform.position.x < -X_BOUND) {
                    StartCoroutine(MoveRobot("l")); // "l" for left

                // Move robot right if target's position is right of the X_BOUND
                } else if (currImageTarget.transform.position.x > X_BOUND) {
                    StartCoroutine(MoveRobot("r")); // "r" for right

                // Move robot backward if target's position is closer than MIN_Z_BOUND
                } else if (currImageTarget.transform.position.z < MIN_Z_BOUND) {
                    StartCoroutine(MoveRobot("b")); // "b" for backward

                // Move robot forward if target's position is further than MAX_Z_BOUND
                } else if (currImageTarget.transform.position.z > MAX_Z_BOUND) {
                    StartCoroutine(MoveRobot("f")); // "f" for forward
                }
            }
        }
    }

    // Coroutine to handle robot movement in the specified direction
    IEnumerator MoveRobot(string direction) {
        // Disable movement temporarily
        canMove = false;

        // Wait until the end of the current frame
        yield return new WaitForEndOfFrame();

        // Send movement direction to the robot
        SendMessageBehavior.Instance.SendPacket(direction);

        // If moving left or right, wait for another frame before sending stop command
        if (direction == "l" || direction == "r") {
            yield return new WaitForEndOfFrame();
        }

        // Wait for another frame
        yield return new WaitForEndOfFrame();

        // Send stop command ("s") to the robot
        SendMessageBehavior.Instance.SendPacket("s");

        // Wait for one more frame before re-enabling movement
        yield return new WaitForEndOfFrame();
        
        // Re-enable movement
        canMove = true;
    }

    // Method to set a new target for the robot to follow
    public void SetNewTarget(GameObject newTracker) {
        // Set the current image target to the provided GameObject’s event handler component
        currImageTarget = newTracker.GetComponent<DefaultTrackableEventHandler>();
    }
}
