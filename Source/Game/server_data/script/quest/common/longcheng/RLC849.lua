--����������10849
--����1���������ˢ��3��С����[3010235],[3010236],[3010237]

--�����ȡʱ����
function q10849_OnAccept(MapID, InstanceID, QuestID, RoleID, NpcID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 3010235, x, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 3010236, x+2, y, z)
	map.MapCreateCreature(MapID, InstanceID, 3010237, x-2, y, z)

end

--ע��
aux.RegisterQuestEvent(10849, 0, "q10849_OnAccept")

