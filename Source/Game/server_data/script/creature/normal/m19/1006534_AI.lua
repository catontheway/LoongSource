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




