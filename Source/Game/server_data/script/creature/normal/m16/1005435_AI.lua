--1005435 ���㣨�����[ǰ]������ս�����ٻ�
    --{1005486 �ٻ���}
	--{1005476 ����ս}
	--{1005472 �ٻ�}/ʾ���ű�
function c1005435_On_Enter_Combat(MapID, InstanceID, CreatureID)
	-- �������ս������
	cre.MonsterSay(MapID, InstanceID, CreatureID, 20071)

       local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 1005486, x-5, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1005476, x+5, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1005472, x, y, z+5)
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end
aux.RegisterCreatureEvent(1005435,2,"c1005435_On_Enter_Combat")

--������������
function c1005435_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")


end

aux.RegisterCreatureEvent(1005435, 4, "c1005435_OnDie")


