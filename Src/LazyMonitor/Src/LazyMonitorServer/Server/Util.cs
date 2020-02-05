using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Server
{
    public class Util
    {
        public static string CreateFilename()
        {
            string filename = DateTime.Now.Hour + "-" + DateTime.Now.Minute + "-" + DateTime.Now.Second;
            return filename;
        }

        public static string CreateSubPath()
        {
            string path = DateTime.Now.ToString("yyyy-MM-dd");
            return path;
        }

        public static void CreateDirectory(string directory)
        {
            if (!Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
            }
        }

    }
}
