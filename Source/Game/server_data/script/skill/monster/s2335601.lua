
--ʹ�ü����ٻ�����ű�

--���磨1290106��ʹ�ü��� ��23345601�� ���ٻ�����(1290116,1290117)


function s2335601_Cast(MapID, InstanceID, SkillID, OwnerID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1290116, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1290117, x, y, z)
	return 0
end


--ע��

aux.RegisterSkillEvent(2335601, 1, "s2335601_Cast")
