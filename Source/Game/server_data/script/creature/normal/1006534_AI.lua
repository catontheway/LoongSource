-- �������ս������ʾ��
function c1006534_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1006534, 2, "c1006534_OnEnterCombat")


function c1006534_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	--��һ�ɱ��ɷʬ���󲥷ŷ������㲥��
	local MsgID = msg.BeginMsgEvent();					--ע��㲥��Ϣ���

	local r = math.random(2);	--ȡ�����
	if ( r==1 ) then
		msg.AddMsgEvent(MsgID,102,100022);
		msg.AddMsgEvent(MsgID,2,RoleID);
	else
		msg.AddMsgEvent(MsgID,102,100023);
		msg.AddMsgEvent(MsgID,2,RoleID);
	end
	msg.DispatchBroadcast(MsgID,3017299919,-1,-1);	--������ˮ�����ڹ㲥
	msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
end
--ע��
aux.RegisterCreatureEvent(1006534, 4, "c1006534_OnDie")




