-- �������ս������ʾ��
--function c1008501_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    --local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    --g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
--end

--aux.RegisterCreatureEvent(1008501, 2, "c1008501_OnEnterCombat")

--��������

function c1008501_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")
	--��һ�ɱ�Ϲ�ħ��[1008501]���ŷ������㲥
	local MsgID = msg.BeginMsgEvent();					--ע��㲥��Ϣ���
	local r = math.random(1,100);	--ȡ�����

	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local Role = {}
	Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	for i=1, 6 do
	    if Role[i] and Role[i]~= 4294967295 then
		    local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
			if Target_MapID == MapID then
			    role.AddRoleItem(Target_MapID, Target_InstanceID, Role[i], 3305003, 5, -1, 8, 420)
			    if act.GetActIsStart(151) then
				    HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
				end
			end
	    end
	end

	msg.AddMsgEvent(MsgID,102,100025);
	msg.AddMsgEvent(MsgID,2,RoleID);
	if ( r<11 ) then
		msg.DispatchBroadcast(MsgID,-1,-1,-1);--ȫ��
	elseif ( r<61 ) then
		msg.DispatchBroadcast(MsgID,3017299151,-1,-1);--����
		msg.DispatchBroadcast(MsgID,3308687566,-1,-1);--������
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);--����
	else
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);--����
	end
end
aux.RegisterCreatureEvent(1008501, 4, "c1008501_OnDie")


