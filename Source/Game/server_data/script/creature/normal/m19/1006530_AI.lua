
-- �������ս������/ʾ���ű�
function c1006530_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 30099)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1006530, 2, "c1006530_OnEnterCombat")

--����ʱ���ڵ�ͼ����<1129, 5696, 975>ˢ�³�������̽î�
--����ʱ����
function c1006530_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 30100)
	map.MapCreateCreature(MapID, InstanceID, 3040522, 1129, 5696, 975)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	local Boss_num=map.GetMapScriptData(MapID, InstanceID, 1, 4)+100
	map.SetMapScriptData(MapID, InstanceID, 1, 4, Boss_num)

	if Boss_num==1111 then
		map.MapCreateCreature(MapID, InstanceID, 1006533, 1509, 10460, 1408)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1746)	--���͵��Ѿ�����������[1509, 1408]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end

end

--ע��
aux.RegisterCreatureEvent(1006530, 4, "c1006530_OnDie")



