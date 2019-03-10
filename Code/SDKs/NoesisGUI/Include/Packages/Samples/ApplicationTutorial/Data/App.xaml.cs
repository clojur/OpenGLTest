﻿#if NOESIS
using Noesis;
using NoesisApp;
#else
using System;
using System.Windows;
#endif

namespace RssReader
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
#if NOESIS
        protected override XamlProvider GetXamlProvider()
        {
            EmbeddedXaml[] xamls =
            {
                new EmbeddedXaml { filename = "App.xaml", resource = "App" },
                new EmbeddedXaml { filename = "MainWindow.xaml", resource = "MainWindow" }
            };
            return new EmbeddedXamlProvider(xamls, Properties.Resources.ResourceManager);
        }

        protected override FontProvider GetFontProvider()
        {
            EmbeddedFont[] fonts =
            {
                new EmbeddedFont { folder = "", resource = "Roboto_Regular" },
                new EmbeddedFont { folder = "", resource = "Roboto_Bold" }
            };
            return new EmbeddedFontProvider(fonts, Properties.Resources.ResourceManager);
        }
#else
        public App()
        {
            // NoesisTheme added to Application.Resources also so it runs exactly the same as inside Noesis
            ResourceDictionary theme = (ResourceDictionary)LoadComponent(new Uri("NoesisTheme.xaml", UriKind.Relative));
            Resources.MergedDictionaries.Insert(0, theme);
        }
#endif
    }
}
