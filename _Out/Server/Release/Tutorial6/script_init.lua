-- the first program in every language
--
--
--
package.path = '../../NFDataCfg/Ini/Components/?.lua;'

function load_script_file(name)
	local object = require(name);
	if nil == object then
		return 0;
	else
		return 1;
	end
end
