
--���ﺰ���ű�

-- �������ս������
function c1005322_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20049)
       end
end

aux.RegisterCreatureEvent(1005322, 2, "c1005322_OnEnterCombat")



--������������
function c1005322_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp1 = math.random(1,100)
	if temp1>=1 and temp1<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 20050)
       end
					local temp = math.random(1,200)
					if temp==1 then
						--��˯��##����������!
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 13, 0)
							msg.AddMsgEvent(MsgID, 1, 364)
							msg.AddMsgEvent(MsgID, 5, 1534331)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 364)
							msg.AddMsgEvent(MsgID, 5, 1534331)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
							--1/200���ʳ���
							map.MapCreateCreature(MapID, InstanceID, 1534331, x, y, z)
					end
end

aux.RegisterCreatureEvent(1005322, 4, "c1005322_OnDie")

