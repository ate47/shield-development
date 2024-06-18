dbflib = {
	source = path.join(dependencies.basePath, "dbflib"),
}

function dbflib.import()
	dbflib.includes()
end

function dbflib.includes()
	includedirs {
		path.join(dbflib.source, "src/lib"),
	}
end

function dbflib.project()

end

table.insert(dependencies, dbflib)
