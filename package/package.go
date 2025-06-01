package cfort

import (
	denv "github.com/jurgen-kluft/ccode/denv"
	ccore "github.com/jurgen-kluft/ccore/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'cfort'
func GetPackage() *denv.Package {
	// Dependencies
	cunittestpkg := cunittest.GetPackage()
	ccorepkg := ccore.GetPackage()

	// The main (cfort) package
	mainpkg := denv.NewPackage("github.com\\jurgen-kluft", "cfort")
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(ccorepkg)

	// 'cfort' library
	mainlib := denv.SetupCppLibProject(mainpkg, "cfort")
	mainlib.AddDependencies(ccorepkg.GetMainLib()...)

	// 'cfort' unittest project
	maintest := denv.SetupCppTestProject(mainpkg, "cfort_test")
	maintest.AddDependencies(cunittestpkg.GetMainLib()...)
	maintest.AddDependencies(ccorepkg.GetMainLib()...)
	maintest.AddDependency(mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
