--���ﺰ���ű�

-- �������ս������/ʾ���ű�
function c1005454_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 20076)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1005454, 2, "c1005454_OnEnterCombat")


--������������
function c1005454_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 20078)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	local Boss_num = map.GetMapScriptData(MapID,InstanceID,1,3)+10
	map.SetMapScriptData(MapID,InstanceID,1,3,Boss_num)

	map.MapCreateCreature(MapID, InstanceID, 3030424, 862, 13431, 1112)

	if Boss_num==1111 then
		map.MapCreateColCreature(MapID, InstanceID, 1005457, 701, 21199, 477, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1727)	--��ū�Ѿ�����������[701, 477]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end

end

aux.RegisterCreatureEvent(1005454, 4, "c1005454_OnDie")

