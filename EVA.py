import argparse
import controller.EvaController as evaController
    
def main():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument(
        '--model',
        help='Name of image classification model.',
        required=False,
        default='model.tflite')
    parser.add_argument(
        '--maxResults',
        help='Max of classification results.',
        required=False,
        default=3)
    parser.add_argument(
        '--scoreThreshold',
        help='The score threshold of classification results.',
        required=False,
        type=float,
        default=0.0)
    parser.add_argument(
        '--numThreads',
        help='Number of CPU threads to run the model.',
        required=False,
        default=4)
    parser.add_argument(
        '--enableEdgeTPU',
        help='Whether to run the model on EdgeTPU.',
        action='store_true',
        required=False,
        default=False)
    parser.add_argument(
        '--cameraId', help='Id of camera.', required=False, default=0)
    parser.add_argument(
        '--frameWidth',
        help='Width of frame to capture from camera.',
        required=False,
        default=500)
    parser.add_argument(
        '--frameHeight',
        help='Height of frame to capture from camera.',
        required=False,
        default=500)
    args = parser.parse_args()
    eva = evaController.EvaController()
    while True:
        eva.run(args.model, int(args.maxResults),args.scoreThreshold, int(args.numThreads), bool(args.enableEdgeTPU),int(args.cameraId), args.frameWidth, args.frameHeight)

if __name__ == '__main__':
    main()
