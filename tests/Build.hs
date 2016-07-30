import Development.Shake
import Development.Shake.Command
import Development.Shake.FilePath
import Development.Shake.Util

import Data.List
import Data.Maybe

import System.Directory

gcc = "gcc"
executableBuildFlags = ["-Wall", "-g"]
testRunnerPath = buildDir </> "runTests" <.> exe

buildDir = "_build"
libDir = ".." </> "source"
libSrcDir = libDir </> "src"
libInclDir = libDir </> "include"
libInclLocDir = libDir </> "include (local)"

testSrcDir = "src"
testInclDir = "include"
testShrdLibsDir = "lib"
testShrdLibsInclDir = testShrdLibsDir </> "include"
headerDirs = [testInclDir, testShrdLibsInclDir, libInclDir, libInclLocDir] -- Non-recursive for now

preDefinedMacros = ["DEBUG"]

objsTestFolderPrefix = buildDir </> "test"
objsLibFolderPrefix = buildDir </> "lib"


-- Pre-calculated varaibles (Don't touch! :D)
headerDirsWithFlag = map ("-iquote" ++) headerDirs
preDefinedMacrosWithFlags = map ("-D " ++) preDefinedMacros

main :: IO ()
main = shakeArgs shakeOptions{shakeFiles="_build"} $ do
    want [testRunnerPath]
    
    phony "clean" $ do
        putNormal "Cleaning files in _build"
        removeFilesAfter "_build" ["//*"]
    
    testRunnerPath %> \out -> do
        putNormal ("Building " ++ testRunnerPath)
        testSrcs <- getDirectoryFiles testSrcDir ["//*.c"]
        libSrcs <- getDirectoryFiles libSrcDir ["//*.c"]
        
        
        let testObjs = map (convertPathSrcToObj (objsTestFolderPrefix)) testSrcs
        let libObjs = map (convertPathSrcToObj (objsLibFolderPrefix)) libSrcs
        let allObjs = testObjs ++ libObjs
        putNormal ("List of sources: " ++ (intercalate ", " (testSrcs ++ libSrcs)))
        putNormal ("List of objects: " ++ (intercalate ", " allObjs))
        
        need allObjs
        cmd gcc executableBuildFlags "-o" out (intercalate " " allObjs)
        
    "//*.o" %> \out -> do
        let normalizedOut = normalise out -- HACK
        let matchingSrc = findSrcForObj normalizedOut
        let srcDependencies = normalizedOut -<.> "m"
        
        putNormal ("Building obj: " ++ normalizedOut)
        putNormal ("Matching src: " ++ matchingSrc)
        
        () <- cmd gcc "-c" [matchingSrc] "-o" [normalizedOut] "-MMD -MF" [srcDependencies] headerDirsWithFlag preDefinedMacrosWithFlags
        needMakefileDependencies srcDependencies
        
convertPathSrcToObj extraFolder path =
    extraFolder </> (path -<.> "o")
    
findSrcForObj objPath
    | beginningOfPath == objsTestFolderPrefix = testSrcDir </> unprefixedDirectory
    | beginningOfPath == objsLibFolderPrefix = libSrcDir </> unprefixedDirectory
    where 
        beginningOfPath = takeDirectory1 objPath </> takeDirectory1 (dropDirectory1 objPath) -- UGLY!!!
        unprefixedDirectory = (dropDirectory1 (dropDirectory1 objPath)) -<.> "c" -- Ughh!
    
