--1006707����ս�� 5�� ��������ˢ������ 1006708

function s2333201_Cast(MapID, InstanceID, SkillID, RoleID)
		  local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
		  map.MapCreateCreature(MapID, InstanceID, 1006708, x, y, z)
end

aux.RegisterSkillEvent(2333201, 1, "s2333201_Cast")
