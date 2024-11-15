# Import necessary libraries
import matplotlib.pyplot as plt  # For displaying images
import numpy as np               # For numerical operations
import cv2                       # OpenCV library for computer vision tasks
%matplotlib inline                # Display plots inline in Jupyter Notebook

# Load the image from the specified path
image1 = cv2.imread('C://Dabir//Documents//Desktop//img.jpg')

# Convert the training image from BGR to RGB color format for display
training_image = cv2.cvtColor(image1, cv2.COLOR_BGR2RGB)

# Convert the training image to grayscale (required for SIFT)
training_gray = cv2.cvtColor(training_image, cv2.COLOR_RGB2GRAY)

# Create a test image by applying scale and rotational transformations
test_image = cv2.pyrDown(training_image)  # Downscale the image (first level)
test_image = cv2.pyrDown(test_image)      # Further downscale the image (second level)
num_rows, num_cols = test_image.shape[:2] # Get dimensions of the downscaled image

# Apply a rotation transformation to add rotational invariance
rotation_matrix = cv2.getRotationMatrix2D((num_cols/2, num_rows/2), 30, 1) # Rotate by 30 degrees
test_image = cv2.warpAffine(test_image, rotation_matrix, (num_cols, num_rows))

# Convert the test image to grayscale
test_gray = cv2.cvtColor(test_image, cv2.COLOR_RGB2GRAY)

# Display both the training and test images side by side
fx, plots = plt.subplots(1, 2, figsize=(20,10))
plots[0].set_title("Training Image")
plots[0].imshow(training_image)   # Show the training image
plots[1].set_title("Testing Image")
plots[1].imshow(test_image)       # Show the test image

# Initialize the SIFT (Scale-Invariant Feature Transform) detector
sift = cv2.xfeatures2d.SIFT_create()

# Detect keypoints and compute descriptors for the training image
train_keypoints, train_descriptor = sift.detectAndCompute(training_gray, None)

# Detect keypoints and compute descriptors for the test image
test_keypoints, test_descriptor = sift.detectAndCompute(test_gray, None)

# Copy of training image to display keypoints without and with size
keypoints_without_size = np.copy(training_image)
keypoints_with_size = np.copy(training_image)

# Draw keypoints without displaying keypoint size
cv2.drawKeypoints(training_image, train_keypoints, keypoints_without_size, color=(0, 255, 0))

# Draw keypoints with size displayed (circle size shows the scale)
cv2.drawKeypoints(training_image, train_keypoints, keypoints_with_size, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# Display images with and without keypoint size
fx, plots = plt.subplots(1, 2, figsize=(20,10))
plots[0].set_title("Train Keypoints With Size")
plots[0].imshow(keypoints_with_size, cmap='gray')   # Display keypoints with size

plots[1].set_title("Train Keypoints Without Size")
plots[1].imshow(keypoints_without_size, cmap='gray') # Display keypoints without size

# Print the number of keypoints detected in the training and test images
print("Number of Keypoints Detected In The Training Image: ", len(train_keypoints))
print("Number of Keypoints Detected In The Query Image: ", len(test_keypoints))

# Create a Brute Force Matcher object
bf = cv2.BFMatcher(cv2.NORM_L1, crossCheck=False)

# Match SIFT descriptors of the training and test images
matches = bf.match(train_descriptor, test_descriptor)

# Sort matches by distance (shorter distance = better match)
matches = sorted(matches, key=lambda x: x.distance)

# Draw the best matches between the training and test images
result = cv2.drawMatches(training_image, train_keypoints, test_gray, test_keypoints, matches, test_gray, flags=2)

# Display the best matching points
plt.rcParams['figure.figsize'] = [14.0, 7.0]
plt.title('Best Matching Points')
plt.imshow(result)
plt.show()

print("\nNumber of Matching Keypoints Between The Training and Query Images: ", len(matches))