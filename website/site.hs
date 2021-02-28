import Hakyll
import Hakyll.Web.Sass
import System.FilePath.Posix (takeBaseName, takeDirectory, (</>))

-- Moves from `/path/to/source.ext` to `/path/to/source/index.html`
-- This way the page can be accessed by `website.tld/path/to/source` without any extension
cleanRoute :: Routes
cleanRoute = customRoute createIndexRoute
  where
    createIndexRoute ident = takeDirectory p </> takeBaseName p </> "index.html"
      where p = toFilePath ident

main :: IO ()
main = hakyll $ do
    match "static/**" $ do
        route idRoute
        compile copyFileCompiler
      
    match "css/*" $ do
        route $ setExtension "css"
        compile $ (compressCss <$>) <$> sassCompiler

    match "index.html" $ do
        route   $ idRoute
        compile $ do
          getResourceBody
            >>= loadAndApplyTemplate "templates/default.html" defaultContext
            >>= relativizeUrls

    match "*.html" $ do
      route $ cleanRoute
      compile $ do
        getResourceBody
          >>= loadAndApplyTemplate "templates/default.html" defaultContext
          >>= relativizeUrls

    match "templates/*" $ compile templateBodyCompiler
