--��ɱ����С��[1007220]��10%����ˢ����������[1007049]

function c1007220_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local r = 0
	r = math.random(1,100)
	if r >90 then
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1007049, x+2, y, z+2)
	end
end
aux.RegisterCreatureEvent(1007220,4,"c1007220_OnDie")
