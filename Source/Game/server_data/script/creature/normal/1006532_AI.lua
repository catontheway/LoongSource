--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1006532_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30103)
			     --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1006532, 2, "c1006532_OnEnterCombat")



--������������
function c1006532_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 30104)
	local a = map.GetMapScriptData(MapID,InstanceID,1,1)
	if a == 1 then
		map.MapCreateCreature(MapID, InstanceID, 1006533, 1509, 10460, 1408)
	else
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
	end
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1006532, 4, "c1006532_OnDie")






