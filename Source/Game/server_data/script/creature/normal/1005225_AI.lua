
--���ﺰ���ű�

-- �������ս������
function c1005225_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20029)
       end	
end

aux.RegisterCreatureEvent(1005225, 2, "c1005225_OnEnterCombat")

