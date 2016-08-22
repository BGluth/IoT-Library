import Development.Shake
import Development.Shake.Command
import Development.Shake.FilePath
import Development.Shake.Util

import Data.List
import Data.Maybe

import qualified System.Directory

gcc = "g++"
executableBuildFlags = ["-Wall"]
objectBuildFlags = ["-g", "-std=c++11"]
testRunnerPath = buildDir </> "runTests" <.> exe

buildDir = "_build"
libDir = ".." </> "source"
libSrcDir = libDir </> "src"
libInclDir = libDir </> "include"
libInclLocDir = libDir </> "include (local)"

testSrcDir = "src"
testInclLocDir = "include (local)"
testShrdLibsDir = "lib"
testShrdLibsInclDir = testShrdLibsDir </> "include"
headerDirs = [testInclLocDir, testShrdLibsInclDir, libInclDir, libInclLocDir] -- Non-recursive for now

preDefinedMacros = []

objsTestFolderPrefix = buildDir </> "test"
objsLibFolderPrefix = buildDir </> "lib"


validSourceFileExtensions = [".c", ".cpp"]

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
        testSrcs <- getDirectoryFiles testSrcDir ["//*.c", "//*.cpp"]
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
        matchingSrc <- liftIO $ findSrcForObj normalizedOut
        let srcDependencies = normalizedOut -<.> "m"
        
        putNormal ("Building obj: " ++ normalizedOut)
        putNormal ("Matching src: " ++ matchingSrc)
        
        () <- cmd gcc "-c" [matchingSrc] "-o" [normalizedOut] objectBuildFlags "-MMD -MF" [srcDependencies] headerDirsWithFlag preDefinedMacrosWithFlags
        needMakefileDependencies srcDependencies
        
convertPathSrcToObj extraFolder path =
    extraFolder </> (path -<.> "o")
    
findSrcForObj objPath = do
    let srcDir = getSrcDirForObjPath objPath
    srcFileName <- findMatchingSrcForObjInSrcDir objPath srcDir
    return (srcDir </> srcFileName)

getSrcDirForObjPath objPath
    | beginningOfPath == objsTestFolderPrefix = testSrcDir </> unprefixedDirectory
    | beginningOfPath == objsLibFolderPrefix = libSrcDir </> unprefixedDirectory
    where 
        beginningOfPath = takeDirectory1 objPath </> takeDirectory1 (dropDirectory1 objPath) -- UGLY!!!
        unprefixedDirectory = takeDirectory (dropDirectory1 (dropDirectory1 objPath)) -- Ughh!
        
findMatchingSrcForObjInSrcDir objPath srcDir = do
    let matchingSrcFileNameNoExt = takeBaseName objPath
    dirContents <- System.Directory.getDirectoryContents srcDir
    let matchingFileNames = filter (\fileInDir -> (dropExtension fileInDir) == matchingSrcFileNameNoExt) dirContents
    let firstMatchingFile = returnFirstFileInListMatchingExtension matchingFileNames validSourceFileExtensions
    return firstMatchingFile
    
returnFirstFileInListMatchingExtension filePathList validExtensions = 
    let 
        extensionIsValid = (\extension -> elem extension validExtensions)
    in
        head (filter (\filePath -> extensionIsValid (takeExtension filePath)) filePathList) -- Should really only ever be one, but I'm not handling this error because I don't have time!
    
