import os
import xml.etree.ElementTree

def FlagsForFile(filename, **kwargs):
    project_name = "TwitchIntegratorSB" # TODO: Your project name here!

    flags = ['-x', 'c++', '-Wall', '-Wextra', '-Werror', '-std=c++11']

    debug = False
    cur_dir = os.path.dirname(os.path.abspath(__file__))

    # Read {project_name}.vcxproj xml file and get definitions and includes
    last_dir = os.curdir
    os.chdir(os.path.join(cur_dir, "Intermediate/ProjectFiles/"))
    proj = xml.etree.ElementTree.parse("{project_name}.vcxproj".format(project_name=project_name)).getroot()

    prefix = "{http://schemas.microsoft.com/developer/msbuild/2003}"
    for g in proj.findall(prefix + "PropertyGroup"):
        definitions = g.find(prefix + "NMakePreprocessorDefinitions")
        if definitions is not None:
            definitions = ["".join(["-D", d]) for d in definitions.text.split(';')]
            if debug:
                for d in definitions[1:]:
                    print(d)
            flags.extend(definitions[1:])

        includes = g.find(prefix + "NMakeIncludeSearchPath")
        if includes is not None:
            incs = []
            for d in includes.text.split(';'):
                if d.endswith("Public") or d.endswith("Classes") or project_name in d:
                    if os.path.isdir(d):
                        incs.append(d)
                    elif debug:
                        print("No such path:" + d)
            include_dirs = ["".join(["-I", os.path.abspath(d).replace("\\", "/")]) for d in incs]

    include_dirs = ["Source/{project_name}/"]
    for dir in include_dirs:
        flags.extend(['-I', os.path.join(cur_dir, dir.format(project_name=project_name))])

    os.chdir(last_dir)

    return {"flags": flags, "do_cache": True}

FlagsForFile("")
