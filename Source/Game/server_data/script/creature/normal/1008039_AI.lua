--���Űħ�� 1008039 ����ս����ˢ�� ��ħС�� 1008040 ���꣺x 1471 y 15188 z 380
--			       ˢ�� ��ħС�� 1008040 ���꣺x 1469 y 15188 z 418
--		           ˢ�� ��ħС�� 1008040 ���꣺x 1434 y 15188 z 417
--			       ˢ�� ��ħС�� 1008040 ���꣺x 1434 y 15188 z 382

function c1008039_On_Enter_Combat(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 1008040, 1471, 15188, 380)
	map.MapCreateCreature(MapID, InstanceID, 1008040, 1469, 15188, 418)
	map.MapCreateCreature(MapID, InstanceID, 1008040, 1434, 15188, 417)
	map.MapCreateCreature(MapID, InstanceID, 1008040, 1434, 15188, 382)
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008039,2,"c1008039_On_Enter_Combat")


--��ɱ�� �Űħ�� 1008039 ˢ�� ����ʯ 3060028 / ������Ч txcj09 ���꣺x 1488 y 15341 z 399
function c1008039_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 3060028, 1488, 15341, 399)

	--ˢ�¼�ħ̨
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1531103, x, y, z)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

end
aux.RegisterCreatureEvent(1008039, 4, "c1008039_OnDie")
