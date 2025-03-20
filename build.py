import os
import subprocess
from pathlib import Path
import argparse

BUILD_PATH = Path('build')


def main() -> int:
    parser = argparse.ArgumentParser(description='build script')
    parser.add_argument('-n','--no-prints',help='Builds without doing log prints, can be slighty faster and not clog up the print space',action='store_true')
    
    parser.add_argument('-v','--vita-ip',nargs='?',default=None,const=None,help='IP of your vita to ftp upload the plugin to ur0:tai')
    
    args = parser.parse_args()
    
    BUILD_PATH.mkdir(exist_ok=True)
    
    os.chdir(BUILD_PATH)
    
    if args.no_prints:
        result = subprocess.run(('cmake','-DNO_PRINTS=1','..'))
    else:
        result = subprocess.run(('cmake','-DNO_PRINTS=0','..'))
    
    if result.returncode:
        return result.returncode
    
    result = subprocess.run(('make',))

    if result.returncode:
        return result.returncode

    if args.vita_ip:
        result = subprocess.run(('ftp','-u',f'ftp://{args.vita_ip}:1337/ur0:/tai/https_checks_disable.suprx','https_checks_disable.suprx'))
        if result.returncode:
            return result.returncode
        
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
