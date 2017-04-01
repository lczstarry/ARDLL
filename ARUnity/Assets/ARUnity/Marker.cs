using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;


namespace UnityAR
{
    public class Marker : MonoBehaviour
    {
        public string name ="";
        public bool is_found;
        public string str = "";
        public string str1 = "";
        public int i = 0;
        public bool mark;

        public float delaytime = 0;

        // Use this for initialization
        void Start()
        {
            name = "RZ";
            is_found = false;
            mark = false;
            string path = GetImagePath();
            pulgins.get_texture_filepath(path);   
        }
 
        // Update is called once per frame
        void Update()
        {

            pulgins.cameraframe_detekeypoints();
            str = pulgins.best_Match().ToString();
           
            // Match();


            //else
            //{
            //    str = "not_found!";
            //}
        }

        void LateUpdate()
        {
            str1 = pulgins.Homography().ToString();
            // StartCoroutine(delay());            
            //  bool  b = false;
            //  b = pulgins.best_Match();
            //   str = b.ToString();      
            // pulgins.best_Match();

        }

        public string GetImagePath()
        {
            return Application.streamingAssetsPath + "/ARUnity/" + name + ".jpg";
        }

        IEnumerator delay()
        {
            // yield return new WaitForSeconds(1f);
            //pulgins.best_Match();
            yield return new WaitWhile(Match);
        }

        private bool Match()
        {
            //  pulgins.cameraframe_detekeypoints();
            // pulgins.best_Match();
            // i = pulgins.Homography();
            // str1 = "comg in here";
            if (pulgins.best_Match())
            {
                str = "true!!!!!!";
                return true;
            }
            else
            {
                str = "false!!!!!!";
                return false;
            }

        }

        void OnGUI()
        {
            str = GUILayout.TextField(str, GUILayout.Width(200), GUILayout.Height(70));
            str1 = GUILayout.TextField(str1, GUILayout.Width(200), GUILayout.Height(70));
        }
    }
}
