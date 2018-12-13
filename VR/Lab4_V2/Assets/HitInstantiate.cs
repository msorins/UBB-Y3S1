using System;
using System.Collections.Generic;

namespace UnityEngine.XR.iOS
{
    public class HitInstantiate : MonoBehaviour
    {
        public List<Transform> trees;
        public List<Transform> smallVegetation;

        public float maxRayDistance = 30.0f;
        public LayerMask collisionLayer = 1 << 10;  //ARKitPlane layer

        bool HitTestWithResultType (ARPoint point, ARHitTestResultType resultTypes)
        {
            List<ARHitTestResult> hitResults = UnityARSessionNativeInterface.GetARSessionNativeInterface ().HitTest (point, resultTypes);
            if (hitResults.Count > 0) {
                foreach (var hitResult in hitResults) {
                    Debug.Log ("Got hit!");

                    Debug.Log(string.Format("distance:{0:0.######}", hitResult.distance));
                    if (hitResult.distance > 0.5) {
                        // Trees
                        var n = trees.Count;
                        var random = Mathf.RoundToInt(Random.Range(0, n - 1));
                        Debug.Log(string.Format("index:{0:0.######}", random));
                        Instantiate(this.trees[random], UnityARMatrixOps.GetPosition(hitResult.worldTransform), UnityARMatrixOps.GetRotation(hitResult.worldTransform));
                    }
                    else {
                        // Small vegetation
                        var n = smallVegetation.Count;
                        var random = Mathf.RoundToInt(Random.Range(0, n - 1));
                        Debug.Log(string.Format("index:{0:0.######}", random));
                        Instantiate(this.smallVegetation[random], UnityARMatrixOps.GetPosition(hitResult.worldTransform), UnityARMatrixOps.GetRotation(hitResult.worldTransform));
                    }

                    return true;
                }
            }
            return false;
        }
        
        // Update is called once per frame
        void Update () {
            #if UNITY_EDITOR   //we will only use this script on the editor side, though there is nothing that would prevent it from working on device
            if (Input.GetMouseButtonDown (0)) {
             
            }
            #else
            if (Input.touchCount > 0)
            {
                var touch = Input.GetTouch(0);
                if (touch.phase == TouchPhase.Began || touch.phase == TouchPhase.Moved)
                {
                    var screenPosition = Camera.main.ScreenToViewportPoint(touch.position);
                    ARPoint point = new ARPoint {
                        x = screenPosition.x,
                        y = screenPosition.y
                    };

                    // prioritize reults types
                    ARHitTestResultType[] resultTypes = {
                        //ARHitTestResultType.ARHitTestResultTypeExistingPlaneUsingGeometry,
                        ARHitTestResultType.ARHitTestResultTypeExistingPlaneUsingExtent, 
                        // if you want to use infinite planes use this:
                        //ARHitTestResultType.ARHitTestResultTypeExistingPlane,
                        //ARHitTestResultType.ARHitTestResultTypeEstimatedHorizontalPlane, 
                        //ARHitTestResultType.ARHitTestResultTypeEstimatedVerticalPlane, 
                        //ARHitTestResultType.ARHitTestResultTypeFeaturePoint
                    }; 
                    
                    foreach (ARHitTestResultType resultType in resultTypes)
                    {
                        if (HitTestWithResultType (point, resultType))
                        {
                            return;
                        }
                    }
                }
            }
            #endif

        }

    
    }
}

