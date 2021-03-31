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

myCompiler = do
  getResourceBody
    >>= loadAndApplyTemplate "template.html" defaultContext
    >>= relativizeUrls

main :: IO ()
main = hakyllWith config $ do
    match "template.html" $ compile templateBodyCompiler

    match "static/**" $ do
      route idRoute
      compile copyFileCompiler
      
    match "style.scss" $ do
      route $ setExtension "css"
      compile $ (compressCss <$>) <$> sassCompiler

    match "index.html" $ do
      route   $ idRoute
      compile $ myCompiler

    match "*.html" $ do
      route $ cleanRoute
      compile $ myCompiler

config :: Configuration
config = defaultConfiguration { providerDirectory = "src" }
