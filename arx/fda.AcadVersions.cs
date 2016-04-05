using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace fda
{
    class AcadVersion 
    {
        public AcadVersion(string name, string ver, string lanCode)
        {
            Name = name;
            Version = ver;
            LanguageCode = lanCode;
        }

        public string Name { get; set; }
        public string Version { get; set; }
        public string LanguageCode { get; set; }

        private AcadVersion() { }
    }

    class AcadVersions
    {
        public static IDictionary<string, AcadVersion> AcadVers
        {
            get { return _AcadVersions; }
        }

        private static IDictionary<string, AcadVersion> _AcadVersions = new Dictionary<string, AcadVersion>()
            {
                {"AutoCAD 2002简体中文版", new AcadVersion("AutoCAD 2002简体中文版", "R15.0", "1:804")},
                {"AutoCAD 2002英文版", new AcadVersion("AutoCAD 2002英文版", "R15.0", "1:409")},
                {"AutoCAD 2004简体中文版", new AcadVersion("AutoCAD 2004简体中文版", "R16.0", "201:804")},
                {"AutoCAD 2004英文版", new AcadVersion("AutoCAD 2004英文版", "R16.0", "201:409")},
                {"AutoCAD 2005简体中文版", new AcadVersion("AutoCAD 2005简体中文版", "R16.1", "301:804")},
                {"AutoCAD 2005英文版", new AcadVersion("AutoCAD 2005英文版", "R16.1", "301:409")},
                {"AutoCAD 2006简体中文版", new AcadVersion("AutoCAD 2006简体中文版", "R16.2", "4001:804")},
                {"AutoCAD 2006英文版", new AcadVersion("AutoCAD 2006英文版", "R16.2", "4001:409")},
                {"AutoCAD 2007简体中文版", new AcadVersion("AutoCAD 2007简体中文版", "R17.0", "5001:804")},
                {"AutoCAD 2007英文版", new AcadVersion("AutoCAD 2007英文版", "R17.0", "5001:409")},
                {"AutoCAD 2008简体中文版", new AcadVersion("AutoCAD 2008简体中文版", "R17.1", "6001:804")},
                {"AutoCAD 2008英文版", new AcadVersion("AutoCAD 2008英文版", "R17.1", "6001:409")},
                {"AutoCAD 2009简体中文版", new AcadVersion("AutoCAD 2009简体中文版", "R17.2", "7001:804")},
                {"AutoCAD 2009英文版", new AcadVersion("AutoCAD 2009英文版", "R17.2", "7001:409")},
                {"AutoCAD 2010简体中文版", new AcadVersion("AutoCAD 2010简体中文版", "R18.0", "8001:804")},
                {"AutoCAD 2010英文版", new AcadVersion("AutoCAD 2010英文版", "R18.0", "8001:409")},
                {"AutoCAD 2011简体中文版", new AcadVersion("AutoCAD 2011简体中文版", "R18.1", "9001:804")},
                {"AutoCAD 2011英文版", new AcadVersion("AutoCAD 2011英文版", "R18.1", "9001:409")},
                {"AutoCAD 2012简体中文版", new AcadVersion("AutoCAD 2012简体中文版", "R18.2", "A001:804")},
                {"AutoCAD 2012英文版", new AcadVersion("AutoCAD 2012英文版", "R18.2", "A001:409")},
                {"AutoCAD 2013简体中文版", new AcadVersion("AutoCAD 2013简体中文版", "R19.0", "B001:804")},
                {"AutoCAD 2013英文版", new AcadVersion("AutoCAD 2013英文版", "R19.0", "B001:409")},
                {"AutoCAD 2014简体中文版", new AcadVersion("AutoCAD 2014简体中文版", "R19.1", "D001:804")},
                {"AutoCAD 2014英文版", new AcadVersion("AutoCAD 2014英文版", "R19.1", "D001:409")},
            };
    }
}
