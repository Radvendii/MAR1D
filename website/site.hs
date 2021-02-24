import           Hakyll
import           Hakyll.Web.Sass

main :: IO ()
main = hakyll $ do
    match "static/**" $ do
        route idRoute
        compile copyFileCompiler
      
    match "css/*" $ do
        route $ setExtension "css"
        compile $ (compressCss <$>) <$> sassCompiler

    match (fromList ["index.html", "downloads/index.html"]) $ do
        route   $ idRoute
        compile $ pandocCompiler
            >>= loadAndApplyTemplate "templates/default.html" defaultContext
            >>= relativizeUrls

    match "templates/*" $ compile templateBodyCompiler


