--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1004532_OnEnterCombat(MapID, InstanceID, CreatureID)

      cre.MonsterSay(MapID, InstanceID, CreatureID, 10072)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1004532, 2, "c1004532_OnEnterCombat")

--��������
function c1004532_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	local YaoJi_ID=map.GetMapScriptData(MapID,InstanceID,1,12)
	unit.CancelBuff(MapID, InstanceID, YaoJi_ID, 9422601)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1738)	--�ٻ��������޵�״̬�ѽ����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

aux.RegisterCreatureEvent(1004532, 4, "c1004532_OnDie")
