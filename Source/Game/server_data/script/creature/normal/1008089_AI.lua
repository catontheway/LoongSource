

-- �������ս������ʾ��
function c1008089_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ5�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008089, 2, "c1008089_OnEnterCombat")

--��������
function c1008089_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")
	--��һ�ɱ��������ħ��[1008089]���Ź㲥
	local MsgID = msg.BeginMsgEvent();					--ע��㲥��Ϣ���
	local r = math.random(1,100);	--ȡ�����

	msg.AddMsgEvent(MsgID,102,100024);
	msg.AddMsgEvent(MsgID,2,RoleID);
	if ( r<11 ) then
		msg.DispatchBroadcast(MsgID,-1,-1,-1);
	elseif ( r<61 ) then
		msg.DispatchBroadcast(MsgID,3017299151,-1,-1);--����
		msg.DispatchBroadcast(MsgID,3308687566,-1,-1);--������
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
	else
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
	end
end
aux.RegisterCreatureEvent(1008089, 4, "c1008089_OnDie")
